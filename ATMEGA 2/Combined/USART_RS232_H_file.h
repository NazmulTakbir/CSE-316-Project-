/*
 * USART_RS232_H_file.h
 * http://www.electronicwings.com
 *
 */ 


#ifndef USART_RS232_H_FILE_H_				/* Define library H file if not defined */
#define USART_RS232_H_FILE_H_


/* Define CPU clock Frequency e.g. here its 8MHz */
#ifndef F_CPU
#define F_CPU 1000000UL
#endif


#include <avr/io.h>							/* Include AVR std. library file */
#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 8UL))) - 1)	/* Define prescale value */

void USART_Init(unsigned long);				/* USART initialize function */
char USART_RxChar();						/* Data receiving function */
void USART_TxChar(char);					/* Data transmitting function */
void USART_SendString(char*);				/* Send string of USART data function */


#endif										/* USART_RS232_H_FILE_H_ */