/*
 * File: MPU_9250.c
 * Purpose: Source File for configuration, calibration, writing, and reading to the MPU_9250
*/

#include "MPU_9250.h"
#include "SPI_Driver.h"
#include "USART_Driver.h"
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
	data: 0x00 -> not needed for config
*/

const struct command configCommandList [configListSize] = {
	{SMPLRT_DIV,9},	
	{INT_PIN_CFG,0x30},
	{INT_ENABLE, 0x01},
	{USER_CTRL, 0x10},
};

void read_MPU_9250 (uint8_t addr, uint8_t bytes, uint16_t * data){
	
}

void write_MPU_9250(struct command cmd){	
	uint8_t data [2] = {cmd.addr, cmd.data};		// partition out the values from the structure to send to the SPIF port
	R_W_SPIF(data,2)								// Write to the MPU9250 via the SPIF port
}
	
void configure_MPU_9250(){

	uint8_t data [2] = {0xF5, 0x00};				// who am i 0x75, bit 8 needs to be 1 for read, returns 0x71
	R_W_SPIF(data, 2);
	write_byte_usartd0(data[1]);
	
	for(int i = 0; i<configListSize; i++){
		write_MPU_9250(configCommandList[i]);		// loop through each register to configure in list and write the command to it
	}
	
	
}
