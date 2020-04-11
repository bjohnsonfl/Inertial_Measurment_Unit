/*
* File: USART_Driver.h
* Purpose: Source file and implementations of USART_Driver.h
*/

#include "USART_Driver.h"

void usartd0_init(){
	PORTD_DIRSET = PIN3_bm;					// tx pin3 out
	PORTD_OUTSET = PIN3_bm;					// set tx pin high for default position
	PORTQ_DIR = 0x0A;						// sets portQ pins 1 and 3 output to enable usb switch and ftdi lines on dev board
	PORTQ_OUTCLR = 0x0A;					// active low enables pins 1 and 3
	USARTD0_CTRLA = USART_DREINTLVL_HI_gc;	// data register empty interrupt enable																
	USARTD0_BAUDCTRLA = 0x0B;				//BSEL[7:0] = 11 0xB baud rate 115200 bps
	USARTD0_BAUDCTRLB = 0x90;				//BSCALE = -7 0b1001, BSEL[11:8] = 0
	// Asych mode, Even parity, 1 stop bit, 8 bit data
	// xmega usart only has 1 start bit, so its non-configureable
	USARTD0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_EVEN_gc | USART_CHSIZE_8BIT_gc;
	USARTD0_CTRLB = USART_TXEN_bm;   // Enable transmitter
	dataRegisterEmpty = 1;				// set flag
}

//Write size amount of bytes out the USARTD0 port
void write_bytes_usartd0(char * data, uint8_t size){
	volatile uint8_t i = 0;						// iterator 
	while(i<size){						// loop through each byte to transmit
		if(USARTD0_STATUS & USART_DREIF_bm){			// poll for interrupt to alert data register is ready to be written to
			USARTD0_STATUS ^= USART_DREIF_bm;	// reset the flag
			USARTD0_DATA = data[i];		// write byte to data register
			i++;
		}
	}
}


//Convert a single float value into a string to be sent to the USARTD0 port
void write_float_usartd0(float data, uint8_t width, uint8_t precision){
	char floatBytes [10];	// buffer to store C string representation of data
	uint8_t size = 0;		// how large the string ends up being
	size = sprintf(floatBytes, "%*.*f",width, precision, data);	// conversion of float to string, returns size
	write_bytes_usartd0(floatBytes, size);	//send to usartd0
}

/*
ISR(USARTD0_DRE_vect){
	USARTD0_STATUS ^= USART_DREIF_bm;   //clear the flag
	dataRegisterEmpty = 1;				// alert that data is ready to write to
}
*/