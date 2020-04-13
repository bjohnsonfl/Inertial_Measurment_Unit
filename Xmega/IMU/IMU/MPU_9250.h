/*
 * File: MPU_9250.h
 * Purpose: Configuration, calibration, writing, and reading to the MPU_9250
*/
#ifndef MPU_9250_H
#define MPU_9250_H

#include <avr/interrupt.h>

// A structure to store the address of the register and the data to write to it  
struct command {
	uint8_t addr;
	uint8_t data;
	};
//A list of commands to configure the MPU_9250	
extern const struct command configCommandList [5];

// Reads information from the device registers. 
// addr is the starting register, bytes is how many registers to read from, data is to store them
void read_MPU_9250 (uint8_t addr, uint8_t bytes, uint16_t * data);

// Write data to a register. 
void write_MPU_9250(struct command cmd);

//initializes internal registers of the MPU_9250 for desirable behavior 
void configure_MPU_9250();

#endif