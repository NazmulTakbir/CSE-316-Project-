/*
 * GccApplication1.c
 *
 * Created: 6/21/2021 9:38:53 PM
 * Author : Nazmul Takbir
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include "USART_RS232_H_file.h"	
#include<avr/wdt.h>
#include<avr/interrupt.h>
#include<util/delay.h>

volatile float distance0;
volatile unsigned char up0 = 0;
volatile uint32_t overflow_counter = 0;
volatile short obstacle=0;

ISR(TIMER1_OVF_vect)
{
	sei();
	if (up0) {
		overflow_counter++;
	}
}

ISR(INT0_vect) {
	sei();
	if (up0 == 0) {
		up0 = 1;
		overflow_counter = 0;
		TCNT1 = 0;
	}
	else {
		up0 = 0;
		distance0 = (overflow_counter*65536+TCNT1)/30.3;  // distance in cm
		if( distance0<200 ) obstacle = 1;
		else obstacle = 0;
	}
}

int main(void)
{
	USART_Init(9600);
	
	DDRA = 0b00000000;
	
	MCUCR &= 0b11111100;
	MCUCR |= 0b00000001;

	GICR |= 0b01000000; 
   
	TCCR1A = 0b00000000; 
	TCCR1B = 0b00000001; 
   
    TIMSK |= 0b00000100; 
	
	char Data_in;
	
    sei();  
	
    while(1)
    {
		Data_in = USART_RxChar();
		
		if(Data_in =='1') {			
			if( obstacle==1 ) {
				if( PINA & 0b00000001 ) USART_TxChar('R');
				else USART_TxChar('L');
			}
			else USART_TxChar('F');
		}
    }
}

