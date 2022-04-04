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
	int flag=0;
	char c;
	
	read_pass(4);
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '#')
		{
			if(!strcmp(USER_PASSWORD,"1234"))
			{
				if (operador1.enable == 1)
				{
					OP = 1;
					flag = 1;
					return 2;	
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
					return 2;
				} else {
					LCD_print2lines("Operador 2 esta","desabilitado");
					delay_3s();				
				}
			}
			else if(!strcmp(USER_PASSWORD,"0123"))
			{
				OP = 3;
				flag = 1;
				return 3;
			} 
			else
			{
				LCD_clear();
				sendString("SENHA INVALIDA!");
				delay_3s();
				read_pass(4);
			}
		}
	}
	
	return 0;
}

//função que faz a leitura da senha
//recebe como parametro o número de caracteres que tem a senha
void read_pass(int n)
{
	int i=0;
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

	while(i<n)
	{	
		c = keyboard_input();
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
		}
	}
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
	char valor[4] = {0,0,0,0};
	int valor_int[4] = {0,0,0,0}, i=0;
	
	LCD_clear();
	LCD_print2lines("Digite o valor","R$         , ");
	
	valor[i] = keyboard_input();
	while(valor[i] != '#')
	{	
		if (valor[i] != '#')
		{
			switch(i)
			{
				case 0: //apenas 1 número digitado
					LCD_clear();
					LCD_print2lines("Digite o valor","R$         , ");		
					sendChar(valor[0]);
					i++;
					valor[i] = keyboard_input();
				break;
			
				case 1: //2 números digitados
					LCD_clear();		
					LCD_print2lines("Digite o valor","R$         ,");	
					sendChar(valor[0]);
					sendChar(valor[1]);
					i++;
					valor[i] = keyboard_input();			
				break;
			
				case 2: //3 números digitados
					LCD_clear();		
					LCD_print2lines("Digite o valor","R$        ");
					sendChar(valor[0]);
					sendChar(',');				
					sendChar(valor[1]);
					sendChar(valor[2]);	
					i++;			
					valor[i] = keyboard_input();				
				break;
			
				case 3: //4 números digitados
					LCD_clear();
					LCD_print2lines("Digite o valor","R$       ");
					sendChar(valor[0]);
					sendChar(valor[1]);
					sendChar(',');
					sendChar(valor[2]);
					sendChar(valor[3]);		
					i++;		
					valor[i] = keyboard_input();				
				break;
			}
		}	
	}

	//Transforma os valores de char para float, para que seja possível
	//realizar operações matemáticas posteriormente
	for(i = 0; i<4 ; i++)
	{
		valor_int[i] = valor[i] - '0';
	}
	PAYMENT_VALUE = valor_int[4]*100 + valor_int[3]*10 + valor_int[2] + valor_int[1]/10 + valor_int[0]/100;	
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

void insere_cartao ()
{
	int flag = 0;
	char c;
	
	read_pass(6);

	while(!flag)
	{
		c = keyboard_input();
		if(c == '#')
		{
			if ((!strcmp(CARD_NUMBER,"283077")) || (!strcmp(CARD_NUMBER,"313799")))
			{
				flag = 1;
				if((STATE == 9) || (STATE == 10))
				{
					STATE = 8;
				}
				if(STATE == 6) 
				{
					STATE = 7;
				}
			} else {
				LCD_clear();
				sendString("CARTAO INVALIDO");
				delay_3s();
				read_pass(6);
			}
		}
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
	int flag=0;
	char c;
	
	//Faz a leitura da senha de 6 digitos do cartão
	read_pass(6);
	
	//Fica aguardando que o usuário clique "confirma"
	while(!flag)
	{
		c = keyboard_input();
		if(c == '#')
		{
			//Verifica se a senha está correta, se tiver, sai do laço
			if(((!strcmp(CARD_PASSWORD,"283077"))&&(!strcmp(CARD_NUMBER,"283077"))) || (!strcmp(CARD_PASSWORD,"313799")&&(!strcmp(CARD_NUMBER,"313799"))))
			{
				flag = 1;
			}
			//Caso a senha esteja errada, é informado ao usuário
			else
			{
				LCD_clear();
				sendString("SENHA INVALIDA");
				delay_3s();
				read_pass(6);				
			}
		}
	}
	
	//Verifica qual dos operador que realizou a operação
	//e adiciona ao seu saldo do dia
	if (!strcmp(USER_PASSWORD, "1234"))
	{
		operador1.saldo += PAYMENT_VALUE;
	}
	
	if (!strcmp(USER_PASSWORD, "2345"))
	{
		operador2.saldo += PAYMENT_VALUE;
	}
	
	//Informa no display, por 3 segundos, que o pagamento foi aprovado
	LCD_print2lines("Pagamento","Aprovado!");
	delay_3s();
	STATE = 0;
}


//FUNÇÕES DO ADMNISTRADOR
void read_mode_adm()
{
	char c;
	int flag = 0;
	
	LCD_clear();
	LCD_print2lines("Modo", "Admiministrador");
	delay_3s();	
	LCD_clear();
	LCD_print2lines("1-OP 2-He 3-Pend","4-D.Pend 5-Rel.");
	delay_3s();
	
	while(!flag)
	{
		c = keyboard_input();
		if(c == '1')
		{
			LCD_print2lines("Hab/Desabilitar", "Operadores");
			delay_3s();
			flag = 1;
			STATE = 11;
		}
		
		if(c == '2')
		{
			LCD_print2lines("Selecionado:", "Alterar Horario");
			delay_3s();
			flag = 1;
			STATE = 12;
		}
		if(c == '3')
		{
			LCD_print2lines("Selecionado:", "Pendencias");
			delay_3s();
			flag = 1;
			STATE = 13;
		}
		if(c == '4')
		{
			LCD_print2lines("Selecionado:", "Limpar Penden.");
			delay_3s();
			flag = 1;
			STATE = 14;
		}
		if(c == '5')
		{
			LCD_print2lines("Selecionado:", "Relatorios");
			delay_3s();
			flag = 1;
			STATE = 15;
		}
	}
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
	int flag = 0, operador = 0;
	float total = 0;
	
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
			
			total = operador1.saldo + operador2.saldo;
			
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
	int flag=0, state;
	char c, i=0, hora[6];
	
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