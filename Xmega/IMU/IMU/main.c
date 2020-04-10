/*
 * IMU.c
 *
 * Created: 4/8/2020 5:17:50 PM
 * Author : Blake
 */ 

#define F_CPU 2000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "SPI_Driver.h"

int main(void)
{
	SPIF_init();
    PMIC_CTRL = PMIC_HILVLEN_bm;
	sei();
	
	volatile uint8_t buff = 0;
	uint8_t data [] = {0xF5, 0x00}; // who_am_I reg, returns 0x71 01110001
	writeBytes(data, sizeof(data));
	buff = SPIF_DATA;
    while (1) 
    {
		//_delay_ms(2000);
		//writeBytes(data, sizeof(data));
    }
}

