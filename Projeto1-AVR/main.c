#include "header.h"

int main(void)
{	
	MICPAY_init();
	LCD_init();
	VAR_init();
	Uart_init();
	
	char c;
	STATE=0;
	static int date_time = 0, debit_credit = 0;
	
	/* Replace with your application code */
    while (1) 
    {
        switch(STATE)
        {
	        case 0: //espera a máquina ser ligada
				LCD_clear();
				//sendString("OFF");
				PORTC &= ~(1 << 4);
				PORTC &= ~(1 << 5);
				UCSR0B = (0 << RXCIE0) | (0 << RXEN0) | (0 << TXEN0); // desabilita com externa.
				//send_command(0xC, 0); 	// desliga o display
				
				c = keyboard_input();
				
				if(c == '#')
				{
					STATE = maquina_on_off(1);
				}
			break;
			
	        case 1: //espera pela senha de usuário
				UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); // desabilita com externa.
				STATE = read_user_password();
	        break;
			
	        case 2: //tela para selecionar a função operador
				LCD_print2lines("1-A Vista 2-A","Prazo 3-Estorno");
				STATE = select_mode();
	        break;
			
	        case 3: //tela de admin
				STATE = read_mode_adm();
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
				STATE = insere_cartao();
	        break;
			
			case 7://Confirmacao Operador
				STATE = confirma_senha_op();
			break;
			
			case 8: //entrada senha cartao
				STATE = senha_cartao();
			break;
			
			case 9: //numero de parcelas
				numero_parcelas();
				//entrada numero cartao
				STATE = insere_cartao();
			break;
			
			case 10: //débito ou credito
				if(debit_credit == 0)
				{
					debit_or_credit();
					debit_credit = 1;
				}
				else if(debit_credit == 1)
				{
					STATE = insere_cartao(); //entrada numero cartao
					
					if(STATE != 10)
						debit_credit = 0;
				}
			break;
			
			case 11:
				STATE = en_dis_op();
			break;
			
			case 12: //Alterar data e hora
				
				if(date_time == 0)
				{
					date_time = 1;
					STATE = altera_data();
					
					if(STATE == 0)
						date_time = 0;
				}
				else if(date_time == 1)
				{
					STATE = altera_hora();
					date_time = 0;
				}
			break;
			
			case 13: //Pendencias
				STATE = menu_pendencias();
			break;
			
			case 14: //Limpa pendencias
				LCD_print2lines("estado","14");
				while(1);
			break;
			
			case 15: //Relatorios
				STATE = relatorios();
			break;
        }
    }
}

