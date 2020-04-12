/*
 * IMU.c
 *
 * Created: 4/8/2020 5:17:50 PM
 * Author : Blake
 */ 

#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



#include "SPI_Driver.h"
#include "USART_Driver.h"

int main(void)
{
	//SPIF_init();							// SPI bus on port f initialized
    usartd0_init();
	PMIC_CTRL = PMIC_HILVLEN_bm;			// enable high level interrupts
	sei();									// turn on interrupts 
	PORTC_DIRSET = PIN0_bm;
	PORTC_OUTCLR = PIN0_bm;
	volatile uint8_t buff = 0;
	uint8_t data [] = {0xF5, 0x00}; // who_am_I reg, returns 0x71 01110001
	//writeBytes(data, sizeof(data));
	
	char charData [] = "   hello world";
	volatile float a = 0.0, b = 3.14, c = 0.2;
	a = b + c;
	char  space = 'k';
    while (1) 
    {
			PORTC_OUTTGL = PIN0_bm;
			write_bytes_usartd0(charData, sizeof(charData));
			for(int i = 0; i < 10000; i++){
				for(int j = 0; j < 100; j++){
				//asm("nop");
				buff = 0;
				}
			}
			
			write_float_usartd0(876.543);
			write_byte_usartd0(space);
			
    }
}

