/*
* File: USART_Driver.h
* Purpose: Source file and implementations of USART_Driver.h
*/

#include "USART_Driver.h"

void usartd0_init(){
	
}

void write_bytes_usartd0(char * data, uint8_t size){
	uint8_t i = 0;						// iterator 
	while(i<size){						// loop through each byte to transmit
		if(dataRegisterEmpty){			// wait for interrupt to alert data register is ready to be written to
			dataRegisterEmpty = 0;		// reset the flag
			USARTD0_DATA = data[i];		// write byte to data register
			i++;						// increment iterator
		}
	}
}

void write_float_usartd0(float data, uint8_t width, uint8_t precision){
	char floatBytes [10];	// buffer to store C string representation of data
	uint8_t size = 0;		// how large the string ends up being
	size = sprintf(floatBytes, "%*.*f",width, precision, data);	// conversion of float to string, returns size
	write_bytes_usartd0(floatBytes, size);	//send to usartd0
}