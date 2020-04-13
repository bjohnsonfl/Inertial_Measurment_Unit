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
	//USARTD0_CTRLA = USART_DREINTLVL_HI_gc;	// data register empty interrupt enable																
	USARTD0_BAUDCTRLA = 0x0B;				//BSEL[7:0] = 11 0xB baud rate 115200 bps
	USARTD0_BAUDCTRLB = 0x90;				//BSCALE = -7 0b1001, BSEL[11:8] = 0
	// Asych mode, Even parity, 1 stop bit, 8 bit data
	// xmega usart only has 1 start bit, so its non-configureable
	USARTD0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_EVEN_gc | USART_CHSIZE_8BIT_gc;
	USARTD0_CTRLB = USART_TXEN_bm;			 // Enable transmitter
	dataRegisterEmpty = 1;					 // set flag
}

void write_byte_usartd0(char data){
	while(!(USARTD0_STATUS & USART_DREIF_bm)){}		// poll for interrupt to alert data register is ready to be written to
	USARTD0_STATUS ^= USART_DREIF_bm;				// reset the flag
	USARTD0_DATA = data;							// write byte to data register
}

//Write size amount of bytes out the USARTD0 port
void write_bytes_usartd0(char * data, uint8_t size){
	volatile uint8_t i = 0;						// iterator 
	while(i<size){								// loop through each byte to transmit
		write_byte_usartd0(data[i]);			// send byte to write byte function
		i++;									// increment to next byte
	}
}


//Convert a single float value into a string to be sent to the USARTD0 port
// - The variable width or precision field (an asterisk \c * symbol) is not realized and will to abort the output.
// -Wl,-u,vfprintf -lprintf_flt -lm use these flags in linker for float capabilities
// -Wl,-u,vfprintf -lprintf_min for minimum float capabilities
void write_float_usartd0(float data){
	char floatBytes [20];							// buffer to store C string representation of data
	uint8_t size = 0;								// how large the string ends up being
	size = sprintf(floatBytes, " %3.3f ", data);	// convert float to c string, return size of c string
	write_bytes_usartd0(floatBytes, size);			// send to usartd0
}

/*
ISR(USARTD0_DRE_vect){
	USARTD0_STATUS ^= USART_DREIF_bm;   //clear the flag
	dataRegisterEmpty = 1;				// alert that data is ready to write to
}
*/