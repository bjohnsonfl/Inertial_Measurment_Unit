/*
 * File: SPI_Driver.h
 * Purpose: All functions related to initializing, writing, and reading from the SPI_F port
*/

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <avr/interrupt.h>

extern volatile uint8_t byteSignal;

void SPIF_init();


/*
	input: "shift" array of bytes, number of bytes to send/receive
	output: input buffer is populated with values to be read from calling function
	purpose: Writes bytes to the SPI port, listens to ISR to send next byte and store the byte read
	todo: this code blocks program flow waiting on byte to be sent, need to release function between these times
	notes: do not need a separate "writeByte" function because there needs to be at least 2 bytes, 1 to write and 1 to read	
*/
void R_W_SPIF (uint8_t* buff, uint8_t size);




ISR(SPIF_INT_vect);


#endif /* SPI_DRIVER_H */