#include "header.h"

int maquina_on_off(char mode)
{
	if(mode == 0) //Para desligar a maquina
	{
		//delay_4s();
		//press_cancel();
		
		return 0;
	}
	if(mode == 1) //Para ligar a maquina
	{
		TIFR1 = (1 << 0);
		delay_3s();
		//press_confirm();
		
		return 1;
	}
	return 0;
}

int read_user_password()
{
	int flag=0;
	char c;
	
	read_4pass();
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '#')
		{
			if(!strcmp(USER_PASSWORD,"1234") || !strcmp(USER_PASSWORD,"2345"))
			{
				flag = 1;
				return 2;
			}
			else if(!strcmp(USER_PASSWORD,"0123"))
			{
				
				flag = 1;
				return 3;
			}
			else
			//if(strcmp(USER_PASSWORD,"1234")!=0 && strcmp(USER_PASSWORD,"2345")!=0 && strcmp(USER_PASSWORD,"0123")!=0)
			{
				LCD_clear();
				sendString("SENHA INVALIDA");
				TIFR1 = (1 << 0);
				delay_3s();
				read_4pass();
			}
		}
	}
	
	return 0;
}

// fazer ela generica
void read_4pass()
{
	int i=0;
	char c;
	
	LCD_print2lines("Digite a senha","Senha:");
	while(i<4)
	{
		c = keyboard_input();
		if(c != '*' && c != '#')
		{
			USER_PASSWORD[i] = c;
			sendChar('*');
			i++;
		}
	}
}

int select_mode()
{
	char c;
	int flag=0;
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '1')
		{
			flag=1;
			return 4;
		}
		if(c == '2')
		{
			flag=1;
			return 5;
		}
		if(c == '3')
		{
			flag=1;
			return 6;
		}
	}
	return 0;
}

void read_price()
{
	// converter char para numero
	// (char)valor[i]/10 + '0';
	char valor[5];
	float valor=0;
	
	
	LCD_print2lines("Digite o valor","R$ 000,00");
	
	
}

void avista_mode()
{
	read_price();
}
