#include "header.h"

#define F_CPU 16000000UL
#define USART_BAUDRATE 19200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

char RX_Buffer[20];
char RX_MSG[20];

void Uart_init()
{
	UBRR0L = BAUD_PRESCALE;
	UBRR0H = (BAUD_PRESCALE >> 8);
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); // habilita transmiss�o/recep��o e interrup��o de recep��o
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // Set: 8data, 1stop bit
}

unsigned char USART_Receive()
{
	while ((UCSR0A & (1 << RXC0)) == 0);
	return UDR0;
}

void USART_Transmit_String (int code)
{
	int i, flag=0;
	char TX_MSG[20];
	
	enable_40S = 1;
	long int pay_int = (long int)(PAYMENT_VALUE*100);
	
	switch(code)
	{
		case 1:			// PAGAMENTO A VISTA ------------------- USART_Transmit_String(1);
		CHECK_PAGAMENTO_AVISTA = 1;
		
		TX_MSG[0] = 'V';
		TX_MSG[1] = 'V';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i];
			TX_MSG[8+i] = CARD_PASSWORD[i];
		}
		
		TX_MSG[14] = ((pay_int/1000)%10) + '0';
		TX_MSG[15] = ((pay_int/100)%10) + '0';
		TX_MSG[16] = ((pay_int/10)%10) + '0';
		TX_MSG[17] = ((pay_int)%10) + '0';
		
		TX_MSG[18] = '\r';
		
		break;
		
		case 2:			// PAGAMENTO PARCELADO ----------------- USART_Transmit_String(2);
		CHECK_COMPRA_PARCELADA = 1;
		
		TX_MSG[0] = 'V';
		TX_MSG[1] = 'P';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i];
			TX_MSG[8+i] = CARD_PASSWORD[i];
		}
		
		TX_MSG[14] = NUM_PARCELAS + '0';
		
		TX_MSG[15] = ((pay_int/1000)%10) + '0';
		TX_MSG[16] = ((pay_int/100)%10) + '0';
		TX_MSG[17] = ((pay_int/10)%10) + '0';
		TX_MSG[18] = ((pay_int)%10) + '0';
		
		TX_MSG[19] = '\r';
		
		break;
		
		case 3:			// ESTORNO ------------------------------- USART_Transmit_String(3);
		CHECK_ESTORNO = 1;
		
		TX_MSG[0] = 'E';
		TX_MSG[1] = 'V';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i];
		}
		
		
		TX_MSG[8] = ((pay_int/1000)%10) + '0';
		TX_MSG[9] = ((pay_int/100)%10) + '0';
		TX_MSG[10] = ((pay_int/10)%10) + '0';
		TX_MSG[11] = ((pay_int)%10) + '0';
		
		TX_MSG[12] = '\r';
		
		break;
		
		case 4:			// VERIFICA PAG AGENDADO ----------------- USART_Transmit_String(4);
		CHECK_PARCELA_AGENDADA = 1;
		
		TX_MSG[0] = 'A';
		TX_MSG[1] = 'P';
		
		for(int j=0; j<10; j++)
		{
			if(parcelas[j].ano == YEAR && parcelas[j].mes == MONTH && parcelas[j].dia == DAY)
			{
				for(i=0; i<6; i++)
				{
					TX_MSG[2+i] = parcelas[j].CARD_N[i];
				}
			}
		}
		
		TX_MSG[8] = ((pay_int/1000)%10) + '0';
		TX_MSG[9] = ((pay_int/100)%10) + '0';
		TX_MSG[10] = ((pay_int/10)%10) + '0';
		TX_MSG[11] = ((pay_int)%10) + '0';
		
		TX_MSG[12] = '\r';
		
		break;
		
		default:
		break;
		
	}
	
	i=0;
	while(!flag)
	{
		while (!( UCSR0A & (1<<UDRE0)));
		UDR0 = TX_MSG[i];
		
		if(TX_MSG[i] == '\r')
		flag=1;
		
		i++;
		
	}
}

/* Recebe o dado da serial e retorna um valor de acordo com a resposta:
 * Retornos:
 * 1 - OK
 * 2 - CF
 * 3 - SF
 * 4 - SI
 * 5 - PN
 * 6 - CM
 * 7 - CW
 */
int USART_Receive_String()
{
	int flag_return, i; // flag do valor de retorno
	PORTC &= ~(1 << 4); // desliga o led da falha de comunica��o
	
	if(RX_MSG[0] == 'O' && RX_MSG[1] == 'K')	// OK = Pagamento efetivado
	{
		//LCD_print2lines("PAGAMENTO","EFETIVADO");
		
		check_receive();
		
		flag_return = 1;
		
	}
	if(RX_MSG[0] == 'C' && RX_MSG[1] == 'F')	// CF = Conta com falha (Inv�lida)
	{
		//LCD_print2lines("CONTA COM","FALHA");
		
		check_receive();
		
		flag_return = 2;
	}
	if(RX_MSG[0] == 'S' && RX_MSG[1] == 'F')	// SF = Senha com falha (Inv�lida)
	{
		//LCD_print2lines("SENHA","INVALIDA");
		
		check_receive();
		
		flag_return = 3;
	}
	if(RX_MSG[0] == 'S' && RX_MSG[1] == 'I')	// SI = Saldo Insuficiente
	{
		//LCD_print2lines("SALDO","INSUFICIENTE");
		
		check_receive();
		
		flag_return = 4;
	}
	if(RX_MSG[0] == 'P' && RX_MSG[1] == 'N')	// PN = Pagamento n�o localizado
	{
		
		if(CHECK_PARCELA_AGENDADA == 1)
		{
			CHECK_PARCELA_AGENDADA = 0;
			PORTC |= (1 << 5); // pagamento est� pendente.
		}
		
		flag_return = 5;
	}
	if(RX_MSG[0] == 'C' && RX_MSG[1] == 'M')	// CM = Envio de n�mero do cart�o magn�tico - CM313799
	{
		
		for(i=0; i<6; i++)
		{
			CARD_NUMBER[i] = RX_MSG[2+i];
		}
		
		//LCD_clear();sendString(RX_MSG);
		
		flag_return = 6;
	}
	if(RX_MSG[0] == 'C' && RX_MSG[1] == 'W')	// CW = Envio de n�mero do cart�o wireless - CW313799313799
	{
		
		for(i=0; i<6; i++)
		{
			CARD_NUMBER[i] = RX_MSG[2+i];
			CARD_PASSWORD[i] = RX_MSG[8+i];
		}
		//LCD_clear(); sendString(CARD_NUMBER); sendString(CARD_PASSWORD);
		
		flag_return = 7;
	}
	
	return flag_return;
}

ISR(USART_RX_vect)
{
	int i, flag=0;
	
	i=0;
	while(!flag)
	{
		RX_Buffer[i] = USART_Receive();
		if(RX_Buffer[i] == '\r')
			flag = 1;
		else
			i++;
	}
	strcpy(RX_MSG, RX_Buffer);
	
	enable_40S = 0;
	COUNT_40S = 0;
	
	USART_Receive_String();
}

void check_receive()
{
	if(CHECK_PAGAMENTO_AVISTA == 1)
	{
		CHECK_PAGAMENTO_AVISTA = 0;
	}
	if(CHECK_COMPRA_PARCELADA == 1)
	{
		CHECK_COMPRA_PARCELADA = 0;
	}
	if(CHECK_ESTORNO == 1)
	{
		CHECK_ESTORNO = 0;
	}
	if(CHECK_PARCELA_AGENDADA == 1)
	{
		CHECK_PARCELA_AGENDADA = 0; // Pagamento da parcela confirmado!
	}
}