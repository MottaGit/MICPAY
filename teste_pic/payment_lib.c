#include "header.h"

void maquina_on_off(char mode)
{
    if(mode == 0) //caso para desligar a maquina
    {
        
    }
    if(mode == 1) //caso para ligar a máquina
    {
        char a;
		while(!(TIFR1 & (1<<0)))
        {
        	a = keyboard_input();
			if(a != '#')
				TCCRB1B = 0;
		}
    }
}
