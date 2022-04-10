#include "header.h"

int maquina_on_off(char mode)
{
	if(mode == 0) //Para desligar a maquina
	{
		return press_cancel();
	}
	if(mode == 1) //Para ligar a maquina
	{
		return press_confirm();
	}
	return 0;
}

int read_user_password()
{
	int flag=0, state;
	char c;
	
	if(read_pass(4) == 0)
	{
		flag = 1;
		state = 0;
	}
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '*')
		{
			if(maquina_on_off(0) == 1)
			{
				flag = 1;
				state = 0;
			}
		}
		if(c == '#')
		{
			if(!strcmp(USER_PASSWORD,"1234"))
			{
				if (operador1.enable == 1)
				{
					OP = 1;
					flag = 1;
					state = 2;	
				} else {
					LCD_print2lines("Operador 1 esta","desabilitado");
					delay_3s();
				}
			}
			else if(!strcmp(USER_PASSWORD,"2345") && operador2.enable == 1)
			{
				if (operador2.enable == 1)
				{
					OP = 2;
					flag = 1;
					state = 2;
				} else {
					LCD_print2lines("Operador 2 esta","desabilitado");
					delay_3s();				
				}
			}
			else if(!strcmp(USER_PASSWORD,"0123"))
			{
				OP = 3;
				flag = 1;
				state = 3;
			} 
			else
			{
				LCD_clear();
				sendString("SENHA INVALIDA!");
				delay_3s();
				
				state = 1;
			}
		}
	}
	
	return state;
}

//função que faz a leitura da senha
//recebe como parametro o número de caracteres que tem a senha
int read_pass(int n)
{
	int i=0, flag = 0, state=1;
	char c;

	if((n == 6)&&(STATE == 6 || STATE == 10 || STATE == 9))
	{
		LCD_clear();
		LCD_print2lines("Numero do","Cartao: ");
	}
	else
	{
		LCD_clear();
		LCD_print2lines("Digite a senha","Senha: ");
	}

	while(!flag)
	{	
		c = keyboard_input();
		
		if(c == '#')
		{
			if(USART_Receive_String() == 6 && n==6)  // se receneu numero do cartao pela serial
			{
				flag = 1;
				state = 2;
			}
			else if(USART_Receive_String() == 7 && n==6) // se receneu numero do cartao e senha pela serial
			{
				flag = 1;
				state = 3;
			}
		}
		
		if(c != '*' && c != '#')
		{
			if (n == 4)
			{
				USER_PASSWORD[i] = c;
				sendChar('*');
				i++;
			}
			if ((n == 6)&&(STATE == 6 || STATE == 10 || STATE == 9))
			{
				CARD_NUMBER[i] = c;
				sendChar(c);
				i++;
			}	
			if ((n == 6)&&(STATE == 8))
			{
				CARD_PASSWORD[i] = c;
				sendChar('*');
				i++;
			}
			if(i == n)
				flag = 1;
		}
		
		if(c == '*')
		{
			if(maquina_on_off(0) == 1)
			{
				flag = 1;
				state = 0;
			}
		}
	}
	
	return state;
}

int select_mode()
{
	char c;
	int flag=0, mode=0;
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '*')
		{
			if(maquina_on_off(0))
			{
				flag = 1;
				mode = 0;
			}
		}
		if(c == '1')
		{
			LCD_print2lines("Selcionado:","A vista");
			delay_3s();
			flag=1;
			mode = 4;
		}
		if(c == '2')
		{
			LCD_print2lines("Selcionado:","A prazo");
			delay_3s();			
			flag=1;
			mode = 5;
		}
		if(c == '3')
		{
			LCD_print2lines("Selcionado:","Estorno");
			delay_3s();
			flag=1;
			mode = 6;
		}
	}
	return mode;
}

void read_price() // VALOR MAXIMO R$99,99
{
	char valor[4] = {0,0,0,0}, c;
	int valor_int[4], i=0, flag = 0;
	
	LCD_clear();
	LCD_print2lines("Digite o valor","R$         , ");
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '#')
		{
			flag = 1;
		}
		if (c != '#')
		{
			switch(i)
			{
				case 0: //apenas 1 número digitado
				LCD_clear();
				LCD_print2lines("Digite o valor","R$         , ");
				valor[i] = c;
				sendChar(valor[i]);
				i++;
				break;
				
				case 1: //2 números digitados
				LCD_clear();
				LCD_print2lines("Digite o valor","R$         ,");
				valor[i] = c;
				sendChar(valor[i-1]);
				sendChar(valor[i]);
				i++;
				break;
				
				case 2: //3 números digitados
				LCD_clear();
				LCD_print2lines("Digite o valor","R$        ");
				valor[i] = c;
				sendChar(valor[i-2]);
				sendChar(',');
				sendChar(valor[i-1]);
				sendChar(valor[i]);
				i++;
				break;
				
				case 3: //4 números digitados
				LCD_clear();
				LCD_print2lines("Digite o valor","R$       ");
				valor[i] = c;
				sendChar(valor[i-3]);
				sendChar(valor[i-2]);
				sendChar(',');
				sendChar(valor[i-1]);
				sendChar(valor[i]);
				i++;
				break;
			}
		}
	}

	//Transforma os valores de char para float, para que seja possível
	//realizar operações matemáticas posteriormente
	for(i = 0; i<4 ; i++)
	{
		valor_int[i] = (int)valor[i] - '0';
	}
	PAYMENT_VALUE = valor_int[0]*10 + valor_int[1] + valor_int[2]/10.0 + valor_int[3]/100.0;
}


void debit_or_credit ()
{
	char c;
	int flag = 0;
	
	LCD_clear();
	LCD_print2lines("1 - Debito", "2 - Credito");
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '1')
		{
			flag = 1;
			OP_PAGAMENTO = 1;
			LCD_print2lines("Selecionado:", "Debito");
			delay_3s();
		}
		if(c == '2')
		{
			flag = 1;
			OP_PAGAMENTO = 2;
			LCD_print2lines("Selecionado:", "Credito");
			delay_3s();
		}
	}
}

void numero_parcelas()
{
	char c;
	int flag = 0;
	
	LCD_clear();
	LCD_print2lines("Parcelas", "2 ou 3:");
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '2')
		{
			LCD_print2lines("Selecionado:", "2 vezes");
			delay_3s();
			NUM_PARCELAS = 2;
			flag = 1;
		}
		
		if(c == '3')
		{
			LCD_print2lines("Selecionado:", "3 vezes");
			delay_3s();
			NUM_PARCELAS = 3;
			flag = 1;
		}
	}
}

int insere_cartao()
{
	int flag = 0, state, flag_read;
	char c;
	
	flag_read = read_pass(6);
	if(flag_read == 0)
	{
		flag = 1;
		state = 0;
	}
	if(flag_read == 2)
	{
		flag = 1;
		state = 8; // ler senha do cartao
		/*
		if (compare_pass(1))
		{
			if((STATE == 9) || (STATE == 10))
			{
				state = 8; // ler senha do cartao
			}
		}
		else
		{
			LCD_clear();
			sendString("CARTAO INVALIDO");
			delay_3s();
			state = 10;
		}
		*/
	}
	if(flag_read == 3)
	{
		flag = 1;
		state = 2; // volta para menu do op
		
		/*
		if (compare_pass(2))
		{
			if((STATE == 9) || (STATE == 10))
			{
				state = 2; // volta para menu do op
			}
		}
		else
		{
			LCD_clear();
			sendString("CARTAO INVALIDO");
			delay_3s();
			state = 10;
		}
		*/
	}

	while(!flag)
	{
		c = keyboard_input();
		if(c == '#')
		{
			/*
			if (compare_pass(1))
			{
				flag = 1;
				if((STATE == 9) || (STATE == 10))
				{
					state = 8; // ler senha do cartao
				}
				if(STATE == 6)
				{
					state = 7;
				}
			}
			else
			{
				LCD_clear();
				sendString("CARTAO INVALIDO");
				delay_3s();
				read_pass(6);
			}
			*/
		}
	}
	return state;
}

int compare_pass(int n)
{
	switch (n)
	{
		//case 1: verifica se o nuemro do cartao eh valido
		case 1:
			if((CARD_NUMBER[0]=='2') && (CARD_NUMBER[1]=='8') && (CARD_NUMBER[2]=='3') && (CARD_NUMBER[3]=='0') && (CARD_NUMBER[4]=='7') && (CARD_NUMBER[5]=='7'))
			{
				return 1;
			} else  if((CARD_NUMBER[0]=='3') && (CARD_NUMBER[1]=='1') && (CARD_NUMBER[2]=='3') && (CARD_NUMBER[3]=='7') && (CARD_NUMBER[4]=='9') && (CARD_NUMBER[5]=='9'))
			{
				return 2;
				} else {
				return 0;
			}
		break;
		//case 2: verifica se a senha do cartao eh valido
		case 2:
			if((CARD_PASSWORD[0]=='2') && (CARD_PASSWORD[1]=='8') && (CARD_PASSWORD[2]=='3') && (CARD_PASSWORD[3]=='0') && (CARD_PASSWORD[4]=='7') && (CARD_PASSWORD[5]=='7' && compare_pass(1) == 1))
			{
				return 1;
			} else  if((CARD_PASSWORD[0]=='3') && (CARD_PASSWORD[1]=='1') && (CARD_PASSWORD[2]=='3') && (CARD_PASSWORD[3]=='7') && (CARD_PASSWORD[4]=='9') && (CARD_PASSWORD[5]=='9') && compare_pass(1) == 2)
			{
				return 1;
				} else {
				return 0;
			}
		break;
	}
}

void confirma_senha_op()
{
	int flag=0;
	char c;
	
	LCD_clear();
	LCD_print2lines("Operador, confirme", "sua senha.");
	delay_3s();
	//Faz a leitura da senha de 4 digitos do operador
	read_pass(4);
	
	//Fica aguardando que o usuário clique "confirma"
	while(!flag)
	{
		c = keyboard_input();
		if(c == '#')
		{
			//Verifica se a senha está correta, se tiver, sai do laço
			if(OP==1 && !strcmp(USER_PASSWORD,"1234"))
			{
				operador1.saldo -= PAYMENT_VALUE;
				flag = 1;
			} 
			else if(OP==2 && !strcmp(USER_PASSWORD,"2345"))
			{
				operador2.saldo -= PAYMENT_VALUE;
				flag = 1;
			}
			//Caso a senha esteja errada, é informado ao usuário
			else
			{
				LCD_clear();
				sendString("SENHA INVALIDA");
				delay_3s();
				read_pass(4);
			}
		} 
	}
	//Confirmação de que o Estorno foi realizado
	LCD_clear();
	LCD_print2lines("Estorno realizado","Com sucesso!");
	delay_3s();
	STATE = 0;
}

void senha_cartao()
{
	int flag=0, state;
	char c;
	
	//Faz a leitura da senha de 6 digitos do cartão
	if(read_pass(6) == 0)
	{
		state = 0;
		flag = 1;
	}
	
	//Fica aguardando que o usuário clique "confirma"
	while(!flag)
	{
		c = keyboard_input();
		if(c == '#')
		{
			USART_Transmit_String(1);
			/*
			//Verifica se a senha está correta, se tiver, sai do laço
			if(compare_pass(2))
			{
				flag = 1;
				agenda_parcelas();
			}
			//Caso a senha esteja errada, é informado ao usuário
			else
			{
				LCD_clear();
				sendString("SENHA INVALIDA");
				delay_3s();
				read_pass(6);
			}*/
			
		}
	}
	
	//Verifica qual dos operador que realizou a operação
	//e adiciona ao seu saldo do dia
	if (OP == 1)
	{
		operador1.saldo = operador1.saldo + PAYMENT_VALUE;
	}
	
	if (OP == 2)
	{
		operador2.saldo = operador2.saldo + PAYMENT_VALUE;
	}
	
	//Informa no display, por 3 segundos, que o pagamento foi aprovado
	LCD_print2lines("Pagamento","Aprovado!");
	delay_3s();
	STATE = 1;
}



//FUNÇÕES DO ADMNISTRADOR
int read_mode_adm()
{
	char c;
	int flag = 0, state;
	
	LCD_clear();
	LCD_print2lines("Modo", "Admiministrador");
	delay_3s();	
	LCD_clear();
	LCD_print2lines("1-OP 2-Data/Hr","3-Pend 4-Rel.");
	delay_3s();
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '1')
		{
			LCD_print2lines("Hab/Desabilitar", "Operadores");
			delay_3s();
			flag = 1;
			state = 11;
		}
		
		if(c == '2')
		{
			LCD_print2lines("Selecionado:", "Alterar Data/Hr");
			delay_3s();
			flag = 1;
			state = 12;
		}
		if(c == '3')
		{
			LCD_print2lines("Selecionado:", "Pendencias");
			delay_3s();
			flag = 1;
			state = 13;
		}
		if(c == '4')
		{
			LCD_print2lines("Selecionado:", "Relatorios");
			delay_3s();
			flag = 1;
			state = 15;
		}
		if(c == '*')
		{
			if(maquina_on_off(0) == 1)
			{
				state = 0;
				flag = 1;
			}
		}
	}
	
	return state;
}

void en_dis_op()
{
	char c;
	int flag = 0, operador = 0;
	
	LCD_clear();
	LCD_print2lines("Selecione:", "1 - OPE1, 2 - OPE2");
	delay_3s();
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '1')
		{
			LCD_print2lines("Selecionado:", "Operador 1");
			delay_3s();
			flag = 1;
			operador = 1;
		}
		
		if(c == '2')
		{
			LCD_print2lines("Selecionado:", "Operador 2");
			delay_3s();
			flag = 1;
			operador = 2;
		}
	}
	
	LCD_clear();
	LCD_print2lines("1 - Habilitar", "2 - Desabilitar");
	delay_3s();
	
	while(flag)
	{
		c = keyboard_input();
		if(c == '1')
		{
			if(operador == 1)
			{
				LCD_print2lines("Operador 1", "Habilitado");
				delay_3s();
				flag = 0;
				operador1.enable = 1;
			}
			if(operador == 2)
			{
				LCD_print2lines("Operador 2", "Habilitado");
				delay_3s();
				flag = 0;
				operador2.enable = 1;
			}
		}
		
		if(c == '2')
		{
			if(operador == 1)
			{
				LCD_print2lines("Operador 1", "Desabilitado");
				delay_3s();
				flag = 0;
				operador1.enable = 0;
			}
			if(operador == 2)
			{
				LCD_print2lines("Operador 2", "Desabilitado");
				delay_3s();
				flag = 0;
				operador2.enable = 0;
			}
		}
	}
}

void relatorios()
{
	char c;
	int flag = 0;
	//float total = 0;
	
	LCD_clear();
	LCD_print2lines("1-Dia atual 2-Por", "Oper. 3-Estornos");
	delay_3s();
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '1')
		{
			LCD_print2lines("Selecionado:", "Relatorio do dia");
			delay_3s();
			
			//total = operador1.saldo + operador2.saldo;
			
		}
		
		if(c == '2')
		{
			LCD_print2lines("Selecionado:", "Relatorio por OPE");
			delay_3s();
			
		}
		
		if(c == '3')
		{
			LCD_print2lines("Selecionado:", "Relatorio po OPE");
			delay_3s();
			
		}
		
	}
}


void relatorio_do_dia() 
{
	
}

int altera_hora()
{
	int flag=0, state, i=0;
	char c, hora[6];
	
	while(!flag)
	{
		if(i==0)
		{
			LCD_clear();
			sendString("Digite a hora:");
			send_command(0xC0,0);
		}
		c = keyboard_input();
		if(c != '#' && c != '*' && i<6)
		{
			hora[i] = c;
			sendChar(c);
			i++;
			if(i==2 || i==4)
				sendChar(':');
			
			if(i == 6)
			{
				if(((hora[0]-'0')*10 + (hora[1]-'0'))>23 || ((hora[2]-'0')*10 + (hora[3]-'0'))>59 || ((hora[4]-'0')*10 + (hora[5]-'0'))>59)
				{
					i=0;
					LCD_print2lines("HORA","INVALIDA");
					delay_3s();
				}
				else
				{
					HOUR = ((hora[0]-'0')*10 + (hora[1]-'0'));
					MINUTE = ((hora[2]-'0')*10 + (hora[3]-'0'));
					SECOND = ((hora[4]-'0')*10 + (hora[5]-'0'));
				}
			}
		}
		if(c == '#' && i==6)
		{
			state = 3;
			flag = 1;
		}
		if(c == '*')
		{
			if(maquina_on_off(0) == 1)
			{
				state = 0;
				flag = 1;
			}
			else
			{
				LCD_clear();
				sendString("Digite a hora:");
				send_command(0xC0,0);
				i--;
				for(int m=0; m<i; m++)
				{
					if(m==2 || m==4)
						sendChar(':');
					
					sendChar(hora[m]);
				}
				if(i==2 || i==4)
					sendChar(':');
			}
		}
	}
	
	return state;
}

int altera_data()
{
	int flag=0, state, i=0;
	char c, data[8];
	
	while(!flag)
	{
		if(i==0)
		{
			LCD_clear();
			sendString("Digite a data:");
			send_command(0xC0,0);
		}
		c = keyboard_input();
		if(c != '#' && c != '*' && i<8)
		{
			data[i] = c;
			sendChar(c);
			i++;
			if(i==2 || i==4)
			{
				sendChar('/');
			}
			
			if(i == 8)
			{
				if( ((data[0]-'0')*10 + (data[1]-'0'))>31 || ((data[2]-'0')*10 + (data[3]-'0'))>12 || (((data[0]-'0')*10 + (data[1]-'0')) >= 29 && ((data[2]-'0')*10 + (data[3]-'0')) == 2 && ((data[4]-'0')*1000 + (data[5]-'0')*100 + (data[6]-'0')*10 + (data[7]-'0'))%4 != 0))
				{
					i=0;
					LCD_print2lines("DATA","INVALIDA");
					delay_3s();
				}
				else
				{
					DAY = ((data[0]-'0')*10 + (data[1]-'0'));
					MONTH = ((data[2]-'0')*10 + (data[3]-'0'));
					YEAR = ((data[4]-'0')*1000 + (data[5]-'0')*100 + (data[6]-'0')*10 + (data[7]-'0'));
				}
			}
		}
		
		if(c == '#' && i == 8)
		{
			state = 12; // estado de destino
			flag = 1;
		}
		
		if(c == '*')
		{
			if(maquina_on_off(0) == 1)
			{
				state = 0;
				flag = 1;
			}
			else
			{
				LCD_clear();
				sendString("Digite a data:");
				send_command(0xC0,0);
				i--;
				for(int m=0; m<i; m++)
				{
					if(m==2 || m==4)
						sendChar('/');
					
					sendChar(data[m]);
				}
				if(i==2 || i==4)
					sendChar('/');
			}
		}
		
	}
	
	return state;
}

// ao ser realizado um pagamento parcelado, cria armazena os dados nas structs de pendencias, com as parcelas que precisam ser pagas
void agenda_parcelas()
{
	int i, ad_parcela;
	
	NUM_PARCELAS = 2;
	PAYMENT_VALUE = 30;
	
	ad_parcela = 0;
	for(i=0; i<10; i++)
	{
		if(parcelas[i].enable == 0 && ad_parcela < NUM_PARCELAS)
		{
			ad_parcela++;
			parcelas[i].enable = 1;
			if(MONTH == 12 && ad_parcela == 1)
			{
				parcelas[i].mes = 1;
				parcelas[i].ano = YEAR + 1;
			}
			else if(MONTH == 12 && ad_parcela == 2)
			{
				parcelas[i].mes = 2;
				parcelas[i].ano = YEAR +1 ;
			}
			else if(MONTH == 11 && ad_parcela == 2)
			{
				parcelas[i].mes = 1;
				parcelas[i].ano = YEAR +1 ;
			}
			else
			{
				parcelas[i].mes = MONTH + ad_parcela;
				parcelas[i].ano = YEAR;
			}
			if(((parcelas[i].mes == 4)||(parcelas[i].mes == 6)||(parcelas[i].mes == 9)||(parcelas[i].mes == 11)) && DAY == 31)
			{
				parcelas[i].dia = 30;
			}
			else if(parcelas[i].mes == 2 && parcelas[i].ano%4 == 0)
			{
				parcelas[i].dia = 29;
			}
			else if(parcelas[i].mes == 2 && parcelas[i].ano%4 != 0)
			{
				parcelas[i].dia = 28;
			}
			else
			{
				parcelas[i].dia = DAY;
			}
			parcelas[i].valor = PAYMENT_VALUE / NUM_PARCELAS;
		}
	}
}

// menu que exibe as pendencias e permete serem desabilitadas
int menu_pendencias()
{
	int i=0, j=0, flag = 0, state, count_pend=0, first=-1;
	char c;
	
	for(i=0; i<10; i++)
	{
		if(parcelas[i].enable == 1)
		{
			count_pend++;
			if(first == -1)
			{
				first = i;
			}
		}
	}
	
	if(count_pend == 0)
	{
		LCD_print2lines("Nenhuma Pend.","Encontrada");
		delay_3s();
		state = 3;
		flag = 1;
	}
	
	i = first;
	while(!flag)
	{
		if(parcelas[i].enable)
		{
			LCD_clear();
			sendString("R$");
			
			sendChar((((int)(parcelas[i].valor*100)/1000)%10) + '0');
			sendChar((((int)(parcelas[i].valor*100)/100)%10) + '0');
			sendChar(',');
			sendChar((((int)(parcelas[i].valor*100)/10)%10) + '0');
			sendChar((((int)(parcelas[i].valor*100))%10) + '0');
			
			char day_d, day_u, month_d, month_u, y_d, y_u;
			
			day_d = ((parcelas[i].dia/10)%10) + '0';
			day_u = (parcelas[i].dia%10) + '0';
			
			month_d = ((parcelas[i].mes/10)%10) + '0';
			month_u = (parcelas[i].mes%10) + '0';
			
			y_d = ((parcelas[i].ano/10)%10) + '0';
			y_u = (parcelas[i].ano%10) + '0';
			
			sendChar(' ');
			sendChar(day_d);
			sendChar(day_u);
			sendChar('/');
			sendChar(month_d);
			sendChar(month_u);
			sendChar('/');
			sendChar(y_d);
			sendChar(y_u);
			
			send_command(0xC0,0);
			
			sendString("desab:0   prox:#");
		}
		
		c = keyboard_input();
		if(c == '#')
		{
			
			int flag2=0;
			while(!flag2)
			{
				j++;
				if(parcelas[j].enable && j<10)
				{
					i = j;
					flag2 = 1;
				}
				if(j == 10)
				{
					state = 3; // volta para o meno de seleção de opções do admin
					flag2 = 1;
					flag = 1;
				}
			}
		}
		if(c == '0')
		{
			parcelas[i].enable = 0;
			
			if(parcelas[i].ano == YEAR && parcelas[i].mes == MONTH && parcelas[i].dia == DAY)
			{
				PORTC &= ~(1<<5);
			}
			
			state = 13;
			flag = 1;
		}
		if(c == '*')
		{
			if(maquina_on_off(0) == 1)
			{
				state = 0;
				flag = 1;
			}
		}
	}
	
	return state;
}

void verifica_pendencia()
{
	//PORTC |= (1 << 5); // pagamento está pendente.
	for(int i = 0; i<10; i++)
	{
		if(parcelas[i].enable)
		{
			if(parcelas[i].ano == YEAR && parcelas[i].mes == MONTH && parcelas[i].dia == DAY)
			{
				USART_Transmit_String(4);
			}
		}
	}
}