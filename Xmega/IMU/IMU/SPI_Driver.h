/*
 * File: SPI_Driver.h
 * Purpose: All functions related to initializing, writing, and reading from the SPI_F port
*/

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <avr/interrupt.h>

volatile uint8_t byteSignal;
/*
	Initialize Port F as SPI Master.
	Pin out:
		0-1, 3: don't care's
		2 : mpu 9250 interrupt line, in
		4 : /SS, out
		5 : MOSI, out  SDA
		6 : MISO, in (default by setting spi master bit) ADO
		7 : SCK, out
	SPI Master, Phase 1, Polarity 1, max 1MHz clk, high interrupt level
*/
void SPIF_init(){
	
	SPIF_CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_3_gc | SPI_PRESCALER_DIV128_gc; 
	SPIF_INTCTRL = SPI_INTLVL_HI_gc;
	PORTF_DIRSET = PIN7_bm | PIN5_bm | PIN4_bm;
	PORTF_OUTSET = PIN4_bm; // set /cs high
	byteSignal = 1;
	
	
	//PORTF_int0
}


/*
	input: array of bytes, number of bytes to send
	purpose: Writes bytes to the SPI port, listens to ISR to send next byte
	todo: this code blocks program flow waiting on byte to be sent, need to release function between these times
*/
void writeBytes (uint8_t* buff, uint8_t size){
	uint8_t i = 0;
	PORTF_OUTCLR = PIN4_bm;						// lower chip select to initiate transaction
	while(i <= size){							// loop for each byte
		if(byteSignal == 1 && i < size){		// enter for each byte to send
			SPIF_DATA = buff[i];				// write to the shift register 
			i++;
			byteSignal = 0;						// unset the flag so the ISR can reset it
		}
		else if(byteSignal == 1 &&  i == size){	// after last byte raise the chip select to signal end of transaction
			PORTF_OUTTGL = PIN4_bm;
			i++;								// increment i to exit loop and function
		}
	}
}




ISR(SPIF_INT_vect){
	byteSignal = 1;
}


#endif /* SPI_DRIVER_H */