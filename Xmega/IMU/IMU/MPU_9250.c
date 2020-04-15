/*
 * File: MPU_9250.c
 * Purpose: Source File for configuration, calibration, writing, and reading to the MPU_9250
*/

#include "MPU_9250.h"
#include "SPI_Driver.h"
#include "USART_Driver.h"

static struct {
	uint16_t xAccel;
	uint16_t yAccel;
	uint16_t zAccel;
	
	uint16_t xGyro;
	uint16_t yGyro;
	uint16_t zGyro;
	
	uint16_t temp;
	
	uint16_t xMagnet;
	uint16_t yMagnet;
	uint16_t zMagnet;
	} rawSensorData = {0};


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
}

void get_Raw_Data(){
	/*
	uint8_t data[2] = {0};
	read_MPU_9250(0x80 ^( ACCEL_XOUT_H + 1), 2, data);
	write_byte_usartd0(data[1]);
	//*/
	uint8_t data [15] = {0x80};
	read_MPU_9250(0x80 ^ ACCEL_XOUT_H, 15, data);	// the 0x80 is to set the read bit, Spi will read from registers ACCEL_XOUT_H to GYRO_ZOUT_L
													//High byte is read first, shifted left one byte, and or'ed with low byte 
	rawSensorData.xAccel = (data[1] << 8) | data[2]; 
	rawSensorData.yAccel = (data[3] << 8) | data[4];
	rawSensorData.zAccel = (data[5] << 8) | data[6];
	
	rawSensorData.temp   = (data[7] << 8) | data[8];

	rawSensorData.xGyro = (data[ 9] << 8) | data[10];
	rawSensorData.yGyro = (data[11] << 8) | data[12];
	rawSensorData.zGyro = (data[13] << 8) | data[14];
	
	
	
	//write_byte_usartd0(data[4]);
	//write_byte_usartd0(data[1]);
	//write_byte_usartd0(data[2]);
	write_uint16_usartd0(rawSensorData.xAccel);
	//*/
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
	PORTC_OUTTGL = PIN0_bm;  // debug for logic analyzer to determine fs from mpu9250
	get_Raw_Data();
}