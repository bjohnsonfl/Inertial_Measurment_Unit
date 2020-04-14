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
#include "MPU_9250.h"

int main(void)
{
	SPIF_init();							// SPI bus on port f initialized
    usartd0_init();
	PMIC_CTRL = PMIC_HILVLEN_bm | PMIC_LOLVLEN_bm;	// enable high level interrupts
	sei();									// turn on interrupts 
	
	//Debug 
	PORTC_DIRSET = PIN0_bm;					// led toggle for logic analyzer debugging
	PORTC_OUTCLR = PIN0_bm;
	volatile uint8_t buff = 0;
	
	
	
	char charData [] = "hello world";
	
	char  space = 'k';
    while (1) 
    {
			//PORTC_OUTTGL = PIN0_bm;
			write_bytes_usartd0(charData, sizeof(charData));
			for(int i = 0; i < 10000; i++){
				for(int j = 0; j < 100; j++){
				buff = 0;
				}
			}
			
			write_byte_usartd0(space);
			
    }
}

