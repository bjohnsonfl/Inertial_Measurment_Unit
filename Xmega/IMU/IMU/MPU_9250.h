/*
 * File: MPU_9250.h
 * Purpose: Configuration, calibration, writing, and reading to the MPU_9250
*/
#ifndef MPU_9250_H
#define MPU_9250_H

#include <avr/interrupt.h>

#define configListSize 7
#define calibrationSamples 16
#define calibrateShift 4			//number of bits of numOfConfigSamples
extern volatile uint8_t sampleCount;	// used to count the first n samples for calibration purposes.
extern volatile uint8_t newData;

enum mpuReg {
	SMPLRT_DIV = 0x19,
	CONFIG,
	GYRO_CONFIG,
	ACCEL_CONFIG,
	
	INT_PIN_CFG = 0x37,
	INT_ENABLE,
	INT_STATUS = 0x3A,
	ACCEL_XOUT_H,
	ACCEL_XOUT_L,
	ACCEL_YOUT_H,
	ACCEL_YOUT_L,
	ACCEL_ZOUT_H,
	ACCEL_ZOUT_L,
	TEMP_OUT_H = 0x41,
	GYRO_XOUT_H = 0x43,
	
	USER_CTRL = 0x6A,
	PWR_MGMT_1,
	PWR_MGMT_2,
		
	WHO_AM_I = 0x75,				// returns 0x71
	XA_OFFSET_H = 0x77,
	XA_OFFSET_L,
	YA_OFFSET_H = 0x7A,
	YA_OFFSET_L,
	ZA_OFFSET_H = 0x7D,
	ZA_OFFSET_L,
	
	
	/* ******  AK8963 Registers below ******** */	
	WIA = 0x00,
	INFO,
	ST1,
	HXH,							// Start of the magnetometers data registers
	
	CNTL = 0x0A			
	};
	


// A structure to store the address of the register and the data to write to it  
struct command {
	enum mpuReg addr;
	uint8_t data;
	};
//A list of commands to configure the MPU_9250	
extern const struct command configCommandList [configListSize];

// Reads information from the device registers. 
// addr is the starting register, bytes = number of registers + 1 (to write the first byte), data is to store them
void read_MPU_9250 (enum mpuReg addr, uint8_t bytes, uint8_t * data);

// Write data to a register. 
void write_MPU_9250(struct command cmd);

// Enables on Accelerometer and Gyroscope (and eventually Magnetometer)
void enableSensors();

// Calls read_MPU_9250 to get sensor data and stores them in rawSensorData struct
void get_Raw_Data();

// Transmits all data from raw data struct via USART to external device
void send_Raw_Data();

// collect n number of samples to average and store in offset registers
void calibrateSensors();

//initializes internal registers of the MPU_9250 for desirable behavior 
void configure_MPU_9250();

#endif