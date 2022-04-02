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
				OP = 1; 
				flag = 1;
				return 2;
			}
			else if(!strcmp(USER_PASSWORD,"2345"))
			{
				OP = 2;
				flag = 1;
				return 2;
			}
			else if(!strcmp(USER_PASSWORD,"0123"))
			{
				flag = 1;
				return 3;
			} 
			else
			{
				LCD_clear();
				sendString("SENHA INVALIDA");
				TIFR1 = (1 << 0);
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
	
	if(STATE)
	LCD_print2lines("Digite a senha","Senha:");
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
			if (n == 6)
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
		if(c == '1')
		{
			LCD_print2lines("Selcionado:","A vista");
			TIFR1 = (1 << 0);
			delay_3s();
			flag=1;
			mode = 4;
		}
		if(c == '2')
		{
			LCD_print2lines("Selcionado:","A prazo");
			TIFR1 = (1 << 0);
			delay_3s();			
			flag=1;
			mode = 5;
		}
		if(c == '3')
		{
			LCD_print2lines("Selcionado:","Estorno");
			TIFR1 = (1 << 0);
			delay_3s();
			flag=1;
			mode = 6;
		}
	}
	return mode;
}

void read_price() // VALOR MAXIMO R$99,99
{
	char valor[5] = {0,0,0,0,0};
	int valor_int[5] = {0,0,0,0,0}, i=0;
	
	LCD_clear();
	LCD_print2lines("Digite o valor","R$         , ");
	
	while(keyboard_input() != '#')
	{	
		valor[i] = keyboard_input();
		if (keyboard_input() != '#')
		{
			switch(i)
			{
				case 0: //apenas 1 número digitado
					LCD_clear();
					LCD_print2lines("Digite o valor","R$         , ");		
					sendChar(valor[0]);
					i++;
				break;
			
				case 1: //2 números digitados
					LCD_clear();		
					LCD_print2lines("Digite o valor","R$         ,");	
					sendChar(valor[0]);
					sendChar(valor[1]);
					i++;				
				break;
			
				case 2: //3 números digitados
					LCD_clear();		
					LCD_print2lines("Digite o valor","R$        ");
					sendChar(valor[0]);
					sendChar(',');				
					sendChar(valor[1]);
					sendChar(valor[2]);	
					i++;							
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
				break;
			
				case 4: //5 números digitados
					LCD_clear();
					LCD_print2lines("Digite o valor","R$      ");
					sendChar(valor[0]);
					sendChar(valor[1]);
					sendChar(valor[2]);
					sendChar(',');
					sendChar(valor[3]);
					sendChar(valor[4]);	
					i++;							
				break;	
			}
		}	
	}

	//Transforma os valores de char para float, para que seja possível
	//realizar operações matemáticas posteriormente
	for(i = 0; i<5 ; i++)
	{
		valor_int[i] = valor[i] - '0';
	}
	VALOR_OPERACAO = valor_int[4]*100 + valor_int[3]*10 + valor_int[2] + valor_int[1]/10 + valor_int[0]/100;	
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
			TIFR1 = (1 << 0);
			delay_3s();
		}
		if(c == '2')
		{
			flag = 1;
			OP_PAGAMENTO = 2;
			LCD_print2lines("Selecionado:", "Credito");
			TIFR1 = (1 << 0);
			delay_3s();
		}
		STATE = 7;
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
			TIFR1 = (1 << 0);
			delay_3s();
			flag = 1;
		}
		
		if(c == '3')
		{
			LCD_print2lines("Selecionado:", "3 vezes");
			TIFR1 = (1 << 0);
			delay_3s();
			flag = 1;
		}
		STATE = 7;
	}
}

void insere_cartao ()
{
	int i=0, flag = 0;
	char c;
	
	LCD_print2lines("Insira/digite","cartao: ");
	while(i<6)
	{
		c = keyboard_input();
		if(c != '*' && c != '#')
		{
			CARD_PASSWORD[i] = c;
			sendChar(c);
			i++;
		}
	}	

	while(!flag)
	{
		c = keyboard_input();
		if(c == '#')
		{
			flag  = 1;
			STATE = 8;
		}
		if (c == '*')
		{
			//DESLIGA
			STATE = 0;
		}
	}
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
			if((!strcmp(CARD_PASSWORD,"283077")) || (!strcmp(CARD_PASSWORD,"313799")))
			{
				flag = 1;
			}
			//Caso a senha esteja errada, é informado ao usuário
			else
			{
				LCD_clear();
				sendString("SENHA INVALIDA");
				TIFR1 = (1 << 0);
				delay_3s();
				read_pass(6);				
			}
		}
	}
	
	//Verifica qual dos operador que realizou a operação
	//e adiciona ao seu saldo do dia
	if (!strcmp(USER_PASSWORD, "1234"))
	{
		operador1.saldo += VALOR_OPERACAO;
	}
	
	if (!strcmp(USER_PASSWORD, "2345"))
	{
		operador2.saldo += VALOR_OPERACAO;
	}
	
	//Informa no display, por 3 segundos, que o pagamento foi aprovado
	LCD_print2lines("Pagamento","Aprovado!");
	TIFR1 = (1 << 0);
	delay_3s();
	STATE = 0;
}