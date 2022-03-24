/*
 * keyboard.c
 */ 
#include "header.h"
#include <avr/io.h>

char keyboard_input()
{
	int i, j, flag=0;
	char key;
	char keyboard[4][3] = {{'1','2','3'},
						   {'4','5','6'},
						   {'7','8','9'},
						   {'*','0','#'}};
	PORTD = 0x70;
				
	while(!flag)
	{
		for(i=0; i<4; i++)
		{
			PORTC = 0x0F; //set PC0, PC1, PC2, PC3 -> 0000 1111
			
			PORTC &= ~(1 << i); //A cada iteração ele zera uma porta
			
			for(j=0; j<3; j++)
			{
				if(!debouncing((PIND & (1 << (4+j)))))	//testa as colunas de PD4, PD5 e PD6
				{
					key = keyboard[i][j];
					flag = 1;
				}
			}
		}
	}
	
	return key;
}

char debouncing(char TECLA)
{
	unsigned char Key_last = 0, Count = 0, Key_now, BOUNCE = 6;
	
	while(Count < BOUNCE)
	{
		delay_debouncing(); 			//delay de 1ms
		
		Key_now = TECLA;
		
		if (Key_now == Key_last)
		{
			Count++;
		}
		else
		{
			Count = 0;
			Key_last = Key_now;
		}
	}
	return Key_now;
}

//Função atraso para o debouce 1ms
//Frequência do timer: 16Mhz / 2 = 8000kHz
void delay_debouncing()
{
	// T_Timer = 1/16MHz = 62.5ns
	// Prescaler: 256-> 16MHz/256 = 62500Hz / T_Timer = 1/62500 = 16us
	//num_contagens = (1E-3)/16E-6 = 62
	TCCR0A = 0;    // Configura modo
	TCCR0B = 0x04; // Configura preescaler
	TCNT0 = 194; //256 - 62 = 194
	
	while ((TIFR0 & (1 << 0)) == 0); //aguarda flag
	TIFR0 |= (1 << 0); //limpa flag
}
