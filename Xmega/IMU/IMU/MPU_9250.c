/*
 * File: MPU_9250.c
 * Purpose: Source File for configuration, calibration, writing, and reading to the MPU_9250
*/

#include "MPU_9250.h"
#include "SPI_Driver.h"
#include "USART_Driver.h"

static struct {
	int16_t xAccel;
	int16_t yAccel;
	int16_t zAccel;
	
	int16_t xGyro;
	int16_t yGyro;
	int16_t zGyro;
	
	int16_t temp;  // 333.87 LSB/degC, 21 degC offset
	
	int16_t xMagnet;
	int16_t yMagnet;
	int16_t zMagnet;
	} rawSensorData = {0};

volatile uint8_t newData = 0;
volatile uint8_t sampleCount = 0;

/*
SMPLRT_DIV 0x19 25:
	sampling frq = fs/(1+smplrt_div)
	data: 0x09 to cut 1khz to 100hz
CONFIG  0x1A 26:
	bit [2:0] = 0b001 for 1KHz gyro sampling freq
	data: 0x01
INT_PIN_CFG 0x37 55:
	bit 7 ACTL 1 for active low 0 for active high, bit 6 1 open drain 0 push pull, bit 5 1 pin held high until cleared
	bit 4 int_anyrd_2clear 1 flag cleared by any read operation, bit 3 0 fsync active high (pin is tied to ground)
	bit 2 0 disables fsync interrupt, pins 1:0 0
	data: 0b00110000 0x30 -> int is active high, push pull, help high until clear, cleared by any read
INT_ENABLE 0x38 56: 
	bit 0 RAW_RDY_EN set to 1 enables the data interrupt
	data: 0x01 -> enables interrupt
USER_CTRL  0x6A 106:
	bit 4 I2C_IF_DIS, set to 1 to disable I2C slave module and only use spi mode, refer to 7.1 in documentation
	data: 0x10 set bit 4 to disable i2c
PWR_MGMT_1 0x6B 107:
	reset, sleep, cycle, gyro standby, clk source
	dataL 0x00 -> not needed for config
PWR_MGMT_2 0x6C 108:
	bits 5-0 disable accelerometer and gyroscope axis if set to 1, 0 on reset, so on is default
	data: 0x3f ->Disable all sensors until MCU interrupt is enabled
*/

const struct command configCommandList [configListSize] = {
	{PWR_MGMT_1, 0x80},
	{USER_CTRL, 0x10},
	{CONFIG, 0x01},
	{SMPLRT_DIV, 0xFF},	 // 9 for 100hz, 0xFF for ~ 4hz
	{INT_PIN_CFG,0x10}, // 0x30, debug 0x10
	{PWR_MGMT_2, 0x3F},
	{INT_ENABLE, 0x01},
	
};

void read_MPU_9250 (enum mpuReg addr, uint8_t bytes, uint8_t * data){
	data[0] = addr;
	R_W_SPIF(data, bytes);
}

void write_MPU_9250(struct command cmd){	
	uint8_t data [2] = {cmd.addr, cmd.data};		// partition out the values from the structure to send to the SPIF port
	R_W_SPIF(data,2);								// Write to the MPU9250 via the SPIF port
}

//Turns on gyroscope and accelerometer
void enableSensors(){
	uint8_t data [2] = {PWR_MGMT_2, 0x00};
	R_W_SPIF(data, 2);
	
	uint8_t data1 [2] = {PWR_MGMT_1, 0x00};
	R_W_SPIF(data1, 2);
}

void get_Raw_Data(){
	
	uint8_t data [15] = {0x80};
	read_MPU_9250(0x80 ^ ACCEL_XOUT_H, 15, data);	// the 0x80 is to set the read bit, Spi will read from registers ACCEL_XOUT_H to GYRO_ZOUT_L
													//High byte is read first, shifted left one byte, and or'ed with low byte 
	rawSensorData.xAccel = ((int16_t)data[1] << 8) | data[2]; 
	rawSensorData.yAccel = (data[3] << 8) | data[4];
	rawSensorData.zAccel = (data[5] << 8) | data[6];
	
	rawSensorData.temp   = (data[7] << 8) | data[8];
		// TEMP_degC = ((TEMP_OUT - RoomTemp_Offset) / Temp_Sensitivity + 21degC
		// Temp _Sensitivity = 333.87
	rawSensorData.xGyro = (data[ 9] << 8) | data[10];
	rawSensorData.yGyro = (data[11] << 8) | data[12];
	rawSensorData.zGyro = (data[13] << 8) | data[14];

	
}

void send_Raw_Data(){
	write_uint16_usartd0(rawSensorData.xAccel);
	write_uint16_usartd0(rawSensorData.yAccel);
	write_uint16_usartd0(rawSensorData.zAccel);
	write_uint16_usartd0(rawSensorData.xGyro);
	write_uint16_usartd0(rawSensorData.yGyro);
	write_uint16_usartd0(rawSensorData.zGyro);
	write_uint16_usartd0(rawSensorData.temp);
	write_byte_usartd0('n');
	newData = 0;
}

void calibrateSensors(){
	newData = 0;									// reset the flag to not enter the if statement initially
	uint8_t numOfSamples = 0;						// reset the sample counter if this is function is being called again
	int32_t xAccelAvg = 0;							// Accumulators for each axis of each sensor
	int32_t yAccelAvg = 0;
	int32_t zAccelAvg = 0;
	int32_t xGyroAvg = 0;
	int32_t yGyroAvg = 0;
	int32_t zGyroAvg = 0;
	uint8_t accelShift = 0, gyroShift = 0;			// shift amounts for LSB/g conversion and averaging
	struct command accelOffset [6];						// address and offset data, 2 commands per axis (high and low byte)
	struct command gyroOffset [6] ;
	
	
	while(numOfSamples < calibrationSamples){		// take n number of samples and sum them up
		if(newData == 1){							// wait on the interrupt
			newData = 0;							// reset flag
			numOfSamples += 1;						// increment number of interrupt/samples that have occurred
			
			xAccelAvg += rawSensorData.xAccel;		// used fixed point math to accumulate a summation of values to be averaged
			yAccelAvg += rawSensorData.yAccel;
			zAccelAvg += rawSensorData.zAccel;
			xGyroAvg += rawSensorData.xGyro;
			yGyroAvg += rawSensorData.yGyro;
			zGyroAvg += rawSensorData.zGyro;
			write_uint16_usartd0(rawSensorData.xAccel);
			write_uint16_usartd0(xAccelAvg >> 16);
			write_uint16_usartd0(xAccelAvg & 0xFFFF);
			write_byte_usartd0('n');
		}
	}
	
	// take the average by dividing by the number of samples
	// The number of samples is a multiple of 2, so a shift right works
	// The Offset registers are +-16 G and have different sizes
	// this requires shifting the offsets to that LSB/g scale
	// The accelerometer +- 16g mode has a 2048 LSB/g scale, but the offset is 15 bits, so you need to shift right 1
	// This means the accel_offset has a 1024 LSB/g scale. +-2g mode has 16,384 LSB/g (2^14) 
	// To align the scales, the offset needs to be shifted 4 to the right to reach 2^10 LSB/g
	// The average is divide by the number of samples, which is a power of two, which is another shift right
	accelShift = 4 + calibrateShift;					
	xAccelAvg = xAccelAvg >> accelShift;			// 4 bits for LSB/g and configShift for average
	
	accelOffset[0].addr = XA_OFFSET_H;
	accelOffset[0].data = (uint8_t)((xAccelAvg >> 7) & 0xFF);		// [14:7]
	accelOffset[1].addr = XA_OFFSET_L;
	accelOffset[1].data = ((uint8_t)xAccelAvg & 0x3F) << 1;		// [6:0] [x]
	
	
		write_uint16_usartd0(rawSensorData.xAccel);
		write_uint16_usartd0(xAccelAvg >> 16);
		write_uint16_usartd0(xAccelAvg & 0xFFFF);
		write_byte_usartd0(accelOffset[0].data);
		write_byte_usartd0(accelOffset[1].data);
		write_byte_usartd0('n');
	
	while(1) {}
	
	
	write_MPU_9250(accelOffset[0]);
	write_MPU_9250(accelOffset[1]);
		
													
							
}
	
void configure_MPU_9250(){

	uint8_t data [2] = {0xF5, 0x00};				// who am i 0x75, bit 8 needs to be 1 for read, returns 0x71
	R_W_SPIF(data, 2);								// read from who am i
	write_byte_usartd0(data[1]);					// display result on usart device
		
	for(int i = 0; i<configListSize; i++){			// configure registers
		write_MPU_9250(configCommandList[i]);		// loop through each register to configure in list and write the command to it
	}

}

// This interrupt is sourced from pin2 on port f from the imu to signal new data is ready to be read
ISR(PORTF_INT0_vect){
	get_Raw_Data();
	newData = 1;
}