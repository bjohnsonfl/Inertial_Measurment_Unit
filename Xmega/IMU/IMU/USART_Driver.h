/*
* File: USART_Driver.h
* Purpose: All functions related to USART Communication 
*/


#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

volatile uint8_t dataRegisterEmpty;

void usartd0_init();

void write_byte_usartd0(char data);
void write_bytes_usartd0(char * data, uint8_t size);
void write_uint16_usartd0(int16_t data);


void write_float_usartd0(float data);

//Data ready interrupt vector
ISR(USARTD0_DRE_vect);

#endif