#include <xc.h>
#include "header.h"

/* delay_LCD
 * atraso de 40us para operações normais do display lcd
 * */
void delay_LCD()
{
	
}

/* DELAY_CLEAR
 * atraso de 1,64ms exclusivo para limpar a tela do display lcd
 * */
void delay_clear()
{
    
}

/*SEND_COMMAND
 *
 */
void send_command(char c, char val_rs)
{
    if(val_rs == 0)
		PORTA &= ~(1 << 4);
	else
		PORTA |= (1 << 4);
	
	PORTB |= (1 << 3);      //E = 1
	PORTB &= 0x0F;
	PORTB |= (c & 0xF0);    // passa os 4 bits mais significativos de C para P1 -> cccc cccc & 1111 0000 = cccc 0000
	PORTB &= ~(1 << 3);	    //E = 0

	delay_LCD();			//atraso de 40us para o lcd
	
	PORTB |= (1 << 3);      //E = 1
	c=c<<4;					//desloca 4 bits pra esquerda cccc bbbb -> bbbb 0000
	PORTB &= 0x0F;
	PORTD |= c;
	PORTB &= ~(1 << 3);	    //E = 0
	
	delay_LCD();			//atraso de 40us para o lcd 
}

/*LCD_init
 * função que inicializa o display LCD
 */
void LCD_init()
{
    send_command(0x28, 0); 	// função de utilização
	send_command(0x06, 0); 	// modo de operação: escreve deslocando o cursor para a direita
	send_command(0x0C, 0); 	// display aceso sem cursor
}

void send_string(char* str)
{
	unsigned char index = 0;
	while(str[index] != 0)
	{
		send_command(str[index], 1);
		index++;
	}
}

// limpa o display e retorna o cursor para o início
void LCD_clear()
{
	send_command(0x01, 0);
	delay_clear();
}