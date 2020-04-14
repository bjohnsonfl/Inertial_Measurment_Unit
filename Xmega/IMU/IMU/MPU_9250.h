/*
 * File: MPU_9250.h
 * Purpose: Configuration, calibration, writing, and reading to the MPU_9250
*/
#ifndef MPU_9250_H
#define MPU_9250_H

#include <avr/interrupt.h>

#define configListSize 4

enum mpuReg {
	SMPLRT_DIV = 0x19,
	CONFIG,
	GYRO_CONFIG,
	ACCEL_CONFIG,
	
	INT_PIN_CFG = 0x37,
	INT_ENABLE,
	
	USER_CTRL = 0x6A,
	PWR_MGMT_1,
	
	WHO_AM_I = 0x75,				// returns 0x71
	
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
extern const struct command configCommandList [5];

// Reads information from the device registers. 
// addr is the starting register, bytes is how many registers to read from, data is to store them
void read_MPU_9250 (enum mpuReg addr, uint8_t bytes, uint16_t * data);

// Write data to a register. 
void write_MPU_9250(struct command cmd);

//initializes internal registers of the MPU_9250 for desirable behavior 
void configure_MPU_9250();

#endif