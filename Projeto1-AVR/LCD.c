/*
 * LCD.c
 *
 * Created: 22/03/2022 23:27:15
 *  Author: vini2
 */ 
#include "header.h"
#include <avr/io.h>

#define RS (PINB & (1 << PINB0))
#define E (PINB & (1 << PINB1))

void delay_lcd()
{	
	TCCR0B = 0x02;				//prescaler de 8, T_timer = 8*T_CPU = 0,5us
	TCNT0 = 176;				//(40us/0,5us) = 80 contagens -> 256-80 = 176
	while(!(TIFR0 & (1<<0)));	//aguarda estouro
	TIFR0 = (1<<0);				//limpa flag de estouro
}

void delay_clear()
{	
	TCCR0B = 0x04;				//prescaler de 256, T_timer = 256*T_CPU = 16us
	TCNT0 = 153;				//(1640us/16us) = 103 contagens -> 256-103 = 153
	while(!(TIFR0 & (1<<0)));	//aguarda estouro
	TIFR0 = (1<<0);				//limpa flag de estouro
}

// inicializa o display LCD
void LCD_init()
{
	PORTB &= ~(1 << 0);
	PORTB &= 0x0F;
	PORTB |= 0x20;
	PORTB |= (1 << 1);
	PORTB &= ~(1 << 1);
	delay_lcd();
	
	send_command(0x28, 0); 	// função de utilização
	send_command(0x06, 0); 	// modo de operação: escreve deslocando o cursor para a direita
	send_command(0xC, 0); 	// display aceso sem cursor
}

// enviar comando para o display
void send_command(char c, char rs)
{
	if(rs == 0)                // RS = 1, configura display; RS = 0, escreve no display
		PORTB &= ~(1 << 0);
	else
		PORTB |= (1 << 0);

	PORTB &= 0x0F;            // AND com 0000 1111 -> limpa os bits (P1_4 P1_5 P1_6 P1_7)
	PORTB |= (c&0xF0);        // passa os 4 bits mais significativos de C para P1 -> cccc cccc & 1111 0000 = cccc 0000
	PORTB |= (1 << 1);
	PORTB &= ~(1 << 1);

	delay_lcd(); // delay de 40us

	c = c<<4;
	PORTB &= 0x0F;
	PORTB |= (c&0xF0);
	PORTB |= (1 << 1);
	PORTB &= ~(1 << 1);

	delay_lcd();            // delay de 40us
}

void sendString(char* str)
{
	unsigned char index = 0;
	
	while(str[index] != 0)
	{
		send_command(str[index], 1);
		index++;
	}
}

void sendChar(char c)
{
	send_command(c, 1);
}

void LCD_print2lines(char *line1, char *line2)
{
	LCD_clear();
	sendString(line1);
	send_command(0xC0,0);
	sendString(line2);
}

// limpa o display e retorna o cursor para o início
void LCD_clear()
{
	send_command(0x01, 0);
	delay_clear();
}
