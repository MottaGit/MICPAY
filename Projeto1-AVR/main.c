#include "header.h"

int main(void)
{
	MICPAY_init();
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
			
	        case 2: //tela para selecionar a função operador
				LCD_print2lines("1-A Vista 2-A","Prazo 3-Estorno");
				STATE = select_mode();
	        break;
			
	        case 3: //tela de admin
				LCD_clear();
				LCD_print2lines("Modo", "Admiministrador");
				TIFR1 = (1 << 0);
				delay_3s();
				LCD_print2lines("1-En_OP 2-Hora","3-Pend. 4-D.Pend");
				//STATE = read_mode_adm();
				while(1);
	        break;
			
	        case 4: //modo pagamento a vista
				read_price();
				STATE = 10;
	        break;
			
	        case 5: //modo pagamento parcelado
				read_price();
				STATE = 9;
	        break;
			
	        case 6: //modo estorno
				read_price();
				LCD_clear();
				LCD_print2lines("Operador,", "por seguranca");
				TIFR1 = (1 << 0);
				delay_3s();
				STATE = read_user_password();
	        break;
			
			case 7: //entrada numero cartao
				insere_cartao();
			break;
			
			case 8: //entrada senha cartao
				senha_cartao();
			break;
			
			case 9: //numero de parcelas
				numero_parcelas();
			break;
			
			case 10: //débito ou credito
				debit_or_credit();
			break;
        }
    }
}

