#include <xc.h>
#include "header.h"

char keyboard_input()
{
    char i, flag = 0, key;
    unsigned char keyboard[4][3] = {{'1','2','3',
                                     '4','5','6',
                                     '7','8','9',
                                     '*','0','#'};
    
    TRISA = 0x0E; //RA1, RA2 e RA3, entradas -> 0000 1110
    
    while(!flag)
    {
        for(i=0; i<4; i++)
        {
        	PORTA = 0xE1; //seta RA0, RA5, RA6 e RA7 para 1 -> 1110 0001
            switch(i)
            {
            	case 0:
            		RA0 = 0;
            	break;
            	case 1:
            		RA5 = 0;
            	break;
            	case 2:
            		RA6 = 0;
            	break;
            	case 3:
            		RA7 = 0;
            	break;
			}
			
			for(j=0; j<3, j++)
			{
				PORTA &= ~(1 << 1+j);
				if(!debouncing(PORTA(1+j))	//testa as colunas de RA1, RA2 e RA3
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
	unsigned char Key_last = 0, Count = 0, Key_now, BOUNCE = 7;
	
	while(Count < 7){
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

//Função atraso para o debouce
//12MHz/4 = 3MHz 
//1/3MHz = 333ns
void delay_debouncing()
{
	T1CON = 0x01; //prescaler de 1 - 0000 0001
    TMR1H = 0xF4; // 1000us/333ns = 3000
    TMR1L = 0x48; // 65536 - 3000 = 62536 -> F448
    
    while((PIR1 & (1 << 0)) == 0);	// se INTCON=1 da overflow e termina contagem
	PIR1 &= ~ (1<<0);
}
