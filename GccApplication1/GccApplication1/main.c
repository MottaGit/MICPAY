/*
 * GccApplication1.c
 *
 * Created: 22/03/2022 23:26:25
 * Author : vini2
 */ 

#include <avr/io.h>
#include "header.h"

int main(void)
{
	//pinos para o LCD
	DDRB |= (1 << 0); //Setting PB0 as output
	DDRB |= (1 << 1); //Setting PB1 as output
	DDRB |= (1 << 4); //Setting PB4 as output
	DDRB |= (1 << 5); //Setting PB5 as output
	DDRB |= (1 << 6); //Setting PB6 as output
	DDRB |= (1 << 7); //Setting PB7 as output
	
	//pinos para o keyboard
	DDRC |= (0 << 0); //Setting PC0 as input
	DDRC |= (0 << 1); //Setting PC1 as input
	DDRC |= (0 << 2); //Setting PC2 as input
	DDRC |= (0 << 3); //Setting PC3 as input
	DDRD |= (0 << 4); //Setting PD4 as output
	DDRD |= (0 << 5); //Setting PD5 as output
	DDRD |= (0 << 6); //Setting PD6 as output
	
	LCD_init();
	LCD_clear();
	sendString("FUNCIONA CARAIO");
	
	/* Replace with your application code */
    while (1) 
    {
		
    }
}

