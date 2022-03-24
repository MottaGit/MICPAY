/*
 * GccApplication1.c
 *
 * Created: 22/03/2022 23:26:25
 * Author : vini2
 */ 

#include <avr/io.h>
#include "header.h"

char STATE=1;

int main(void)
{
	PINS_init();
	LCD_init();
	
	TCCR1A = 0; //Modo normal
	TCCR1B = 0x5; //Prescaler 1024
	
	/* Replace with your application code */
    while (1) 
    {
        switch(STATE)
        {
	        case 0: // espera a máquina ser ligada
				LCD_clear();
				sendString("DESLIGADO");
				if(keyboard_input() == '#')
				{
					STATE = maquina_on_off(1);
				}
			break;
			
	        case 1: // espera pela senha de user
				LCD_print2lines("Digite a senha","Senha:");
				STATE = read_user_password();
				while(1);
	        break;
			
	        case 2:
				LCD_print2lines("1-A Vista 2-A","prazo 3-Estorno");
				while(1);
	        break;
			
	        case 3:
	        break;
	        case 4:
	        break;
        }
    }
}

