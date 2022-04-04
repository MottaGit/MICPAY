#include "header.h"

int main(void)
{
	MICPAY_init();
	LCD_init();
	VAR_init();
	
	char c;
	STATE=0;
	
	/* Replace with your application code */
    while (1) 
    {
        switch(STATE)
        {
	        case 0: //espera a máquina ser ligada
				LCD_clear();
				sendString("OFF");
				c = keyboard_input();
				
				if(c == '#')
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
				read_mode_adm();
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
				//entrada numero cartao
				insere_cartao();
	        break;
			
			case 7://Confirmacao Operador
				confirma_senha_op();
			break;
			
			case 8: //entrada senha cartao
				senha_cartao();
			break;
			
			case 9: //numero de parcelas
				numero_parcelas();
				//entrada numero cartao
				insere_cartao();
			break;
			
			case 10: //débito ou credito
				debit_or_credit();
				//entrada numero cartao
				insere_cartao();
			break;
			
			case 11:
				en_dis_op();
			break;
			
			case 12: //Alterar data e hora
				STATE = altera_hora();
			break;
			
			case 13: //Pendencias
				LCD_print2lines("estado","13");
				while(1);
			break;
			
			case 14: //Limpa pendencias
				LCD_print2lines("estado","14");
				while(1);
			break;
			
			case 15: //Relatorios
				//relatorios();
			break;
        }
    }
}

