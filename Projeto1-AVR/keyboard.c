 #include "header.h"

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
				if(!debouncing((PIND & (1 << (4+j) ) )))	//testa as colunas de PD4, PD5 e PD6
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
