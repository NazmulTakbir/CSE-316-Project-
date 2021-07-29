/*
 * GccApplication1.c
 *
 * Created: 6/21/2021 9:38:53 PM
 * Author : Nazmul Takbir
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

volatile float distance0;
volatile float distance1;
volatile unsigned char up0 = 0;
volatile unsigned char up1 = 0;
volatile unsigned char running0 = 0;
volatile unsigned char running1 = 0;
volatile unsigned char overflow_counter = 0;
volatile unsigned char timer0 = 0;
volatile unsigned char timer1 = 0;

ISR(TIMER1_OVF_vect)
{
	sei();
	if (up0||up1) {
		overflow_counter++;
	}
}

ISR(INT0_vect) {
	sei(); 
	if(running0){
		if (up0 == 0) {
			up0 = 1;
			timer0 = overflow_counter;
		}
		else {
			up0 = 0;
			distance0 = ((overflow_counter-timer0)*65536+TCNT1)/30.3;  // distance in cm
			running0 = 0;
		}
	}
}

ISR(INT1_vect){
	sei(); 
	if(running1){ 
		if (up1 == 0) { 
			up1 = 1;
			timer1 = overflow_counter;
		} 
		else { 
			up1 = 0;
			distance1 = ((overflow_counter-timer1)*65536+TCNT1)/30.3;  // distance in cm
			running1 = 0;
		}
	}
}

void send_trigger() {
	PORTD &= 0b10111111;
	_delay_us(5);
	PORTD |= 0b01000000;
	running0 = 1;
	running1 = 1;
	_delay_us(10);
	PORTD &= 0b10111111;
}

int main(void)
{
    DDRD |= 0b01000000;  
	
    PORTD &= 0b10111111;
	
	DDRB |= 0b00000011;
    
	MCUCR &= 0b11110000;
	MCUCR |= 0b00000101;

	GICR |= 0b11000000; 
   
	TCCR1A = 0b00000000; 
	TCCR1B = 0b00000001; 
   
    TIMSK |= 0b00000100; 
	
    sei();  
    while(1)
    {
		overflow_counter = 0;
		TCNT1 = 0;
		send_trigger();
		
		_delay_ms(30);
		
		if( running0==0 && running1==0 ) {
			if( distance0>distance1 ) PORTB = 0b00000001;
			else PORTB = 0b00000000;
		}
		else if( running0==0 ) {
			PORTB = 0b00000000;
		}
		else if( running1==0 ) {
			PORTB = 0b00000001;
		}
		else PORTB = 0b00000000;
		
    }
}