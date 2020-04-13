/*
 * File: MPU_9250.c
 * Purpose: Source File for configuration, calibration, writing, and reading to the MPU_9250
*/

#include "MPU_9250.h"
#include "SPI_Driver.h"
#include "USART_Driver.h"


const struct command configCommandList [5] = {
	{1,2},	
	{3,4},
	{5,6},
	{7,8},
	{9,10},
};

void read_MPU_9250 (uint8_t addr, uint8_t bytes, uint16_t * data){
	
}

void write_MPU_9250(struct command cmd){
	
}
	
void configure_MPU_9250(){
	for(int i = 0; i < 5; i++){
		char data  [2] = {configCommandList[i].addr + 48, configCommandList[i].data + 48};
		//write_bytes_usartd0(data , 2); //48 is 0 in ascii
	}
	uint8_t data [2] = {0xF5, 0x00};  // who am i 0x75, bit 8 needs to be 1 for read, returns 0x71
	R_W_SPIF(data, 2);
	write_byte_usartd0(data[1]);
	
}
