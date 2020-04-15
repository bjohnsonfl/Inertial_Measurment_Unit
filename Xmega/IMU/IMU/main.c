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

void poorlyDoneDelay();

int main(void)
{
	SPIF_init();							// SPI bus on port f initialized
    usartd0_init();
	PMIC_CTRL = PMIC_HILVLEN_bm | PMIC_LOLVLEN_bm;	// enable high level interrupts
	sei();									// turn on interrupts 
	//Debug 
	PORTC_DIRSET = PIN0_bm;					// led toggle for logic analyzer debugging
	PORTC_OUTCLR = PIN0_bm;
	
	_delay_ms(40);							// wait for sensors to start up before configuring
	configure_MPU_9250();
	enableSensors();						// start sensors, interrupts will happen now
	
	
	
	
	
    while (1) 
    {

			
			poorlyDoneDelay();		// simulate processing delay
			
			
    }
}

void poorlyDoneDelay(){
	volatile uint8_t buff = 0;
	for(int i = 0; i < 10000; i++){
		for(int j = 0; j < 100; j++){
			buff = 0;
		}
	}
}