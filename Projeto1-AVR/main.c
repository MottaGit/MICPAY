#include "header.h"

int main(void)
{
	PINS_init();
	LCD_init();
	VAR_init();
	
	STATE=0;
	
	/* Replace with your application code */
    while (1) 
    {
        switch(STATE)
        {
	        case 0: //espera a máquina ser ligada
				LCD_clear();
				sendString("OFF");
				if(keyboard_input() == '#')
				{
					STATE = maquina_on_off(1);
				}
			break;
			
	        case 1: //espera pela senha de usuário
				STATE = read_user_password();
	        break;
			
	        case 2: //tela para selecionar a função
				LCD_print2lines("1-A Vista 2-A","prazo 3-Estorno");
				STATE = select_mode();
	        break;
			
	        case 3: //tela de admin
				LCD_print2lines("modo","admin");
				while(1);
	        break;
			
	        case 4: //modo pagamento a vista
				read_price();
				//LCD_print2lines("modo","pag a vista");
				while(1);
	        break;
			
	        case 5: //modo pagamento parcelado
				LCD_print2lines("modo","pag parcelado");
				while(1);
	        break;
			
	        case 6: //modo estorno
				LCD_print2lines("modo","estorno");
				while(1);
	        break;
        }
    }
}

