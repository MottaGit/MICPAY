#include "header.h"
#include <avr/io.h>
#include <string.h>

int maquina_on_off(char mode)
{
	if(mode == 0) //Para desligar a maquina
	{
		delay_4s();
		
		return 0;
	}
	if(mode == 1) //Para ligar a maquina
	{
		
		delay_3s();
		
		return 1;
	}
	return 0;
}

int read_user_password()
{
	int i=0, flag=0;
	char pass[4], c;
	
	//while(!flag);
	
	while(i<4)
	{
		c = keyboard_input();
		if(c != '*' && c != '#')
		{
			pass[i] = c;
			sendChar(pass[i]);
			i++;
		}
	}
	
	if(keyboard_input() == '#')
	{
		if(!strcmp(pass,"1234"))
		{
			flag = 1;
			sendChar('a');
		}
		else
		{
			sendChar('b');
		}
	}
		
}
