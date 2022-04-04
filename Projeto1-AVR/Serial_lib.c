#include "header.h"

#define F_CPU 16000000UL
#define USART_BAUDRATE 19200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

char RX_Buffer[15];
char RX_MSG[15];

void Uart_init()
{
	UBRR0L = BAUD_PRESCALE;
	UBRR0H = (BAUD_PRESCALE >> 8);
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); // habilita transmissão/recepção e interrupção de recepção
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
	
	switch(code)
	{
		case 1:			// PAGAMENTO A VISTA ------------------- USART_Transmit_String(1)
		TX_MSG[0] = 'V';
		TX_MSG[1] = 'V';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i] + '0';
			TX_MSG[8+i] = CARD_PASSWORD[i] + '0';
		}
		
		TX_MSG[14] = (((int)(PAYMENT_VALUE*100)/1000)%10) + '0';
		TX_MSG[15] = (((int)(PAYMENT_VALUE*100)/100)%10 + '0');
		TX_MSG[16] = (((int)(PAYMENT_VALUE*100)/10)%10) + '0';
		TX_MSG[17] = (((int)PAYMENT_VALUE*100)%10) + '0';
		
		TX_MSG[18] = '\r';
		
		break;
		
		case 2:			// PAGAMENTO PARCELADO ----------------- USART_Transmit_String(2)
		TX_MSG[0] = 'V';
		TX_MSG[1] = 'P';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i] + '0';
			TX_MSG[8+i] = CARD_PASSWORD[i] + '0';
		}
		
		TX_MSG[14] = NUM_PARCELAS + '0';
		
		TX_MSG[15] = (((int)(PAYMENT_VALUE*100)/1000)%10) + '0';
		TX_MSG[16] = (((int)(PAYMENT_VALUE*100)/100)%10) + '0';
		TX_MSG[17] = (((int)(PAYMENT_VALUE*100)/10)%10) + '0';
		TX_MSG[18] = (((int)PAYMENT_VALUE*100)%10) + '0';
		
		TX_MSG[19] = '\r';
		
		break;
		
		case 3:			// ESTORNO ------------------------------- USART_Transmit_String(3)
		TX_MSG[0] = 'E';
		TX_MSG[1] = 'V';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i] + '0';
		}
		
		TX_MSG[8] = (((int)(PAYMENT_VALUE*100)/1000)%10) + '0';
		TX_MSG[9] = (((int)(PAYMENT_VALUE*100)/100)%10) + '0';
		TX_MSG[10] = (((int)(PAYMENT_VALUE*100)/10)%10) + '0';
		TX_MSG[11] = (((int)PAYMENT_VALUE*100)%10) + '0';
		
		TX_MSG[12] = '\r';
		
		break;
		
		case 4:			// VERIFICA PAG AGENDADO ----------------- USART_Transmit_String(4)
		TX_MSG[0] = 'A';
		TX_MSG[1] = 'P';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i] + '0';
		}
		
		TX_MSG[8] = (((int)(PAYMENT_VALUE*100)/1000)%10) + '0';
		TX_MSG[9] = (((int)(PAYMENT_VALUE*100)/100)%10) + '0';
		TX_MSG[10] = (((int)(PAYMENT_VALUE*100)/10)%10) + '0';
		TX_MSG[11] = (((int)PAYMENT_VALUE*100)%10) + '0';
		
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
		
		//delay??
	}
}

int USART_Receive_String()
{
	int flag_return, i; // flag do valor de retorno
	
	if(RX_MSG[0] == 'O' && RX_MSG[1] == 'K')	// OK = Pagamento efetivado
	{
		LCD_print2lines("PAGAMENTO","EFETIVADO");
		LCD_clear();sendString(RX_MSG);
		flag_return = 1;
	}
	if(RX_MSG[0] == 'C' && RX_MSG[1] == 'F')	// CF = Conta com falha (Inválida)
	{
		LCD_print2lines("CONTA COM","FALHA");
		flag_return = 2;
	}
	if(RX_MSG[0] == 'S' && RX_MSG[1] == 'F')	// SF = Senha com falha (Inválida)
	{
		LCD_print2lines("SENHA","INVALIDA");
		flag_return = 3;
	}
	if(RX_MSG[0] == 'S' && RX_MSG[1] == 'I')	// SI = Saldo Insuficiente
	{
		LCD_print2lines("SALDO","INSUFICIENTE");
		flag_return = 4;
	}
	if(RX_MSG[0] == 'P' && RX_MSG[1] == 'N')	// PN = Pagamento não localizado
	{
		LCD_print2lines("PG NAO","LOCALIZADO");
		flag_return = 5;
	}
	if(RX_MSG[0] == 'C' && RX_MSG[1] == 'M')	// CM = Envio de número do cartão magnético
	{
		for(i=0; i<6; i++)
		{
			CARD_NUMBER[i] = RX_MSG[2+i] - '0';
		}
		
		LCD_clear();sendString(RX_MSG);
		
		flag_return = 6;
	}
	if(RX_MSG[0] == 'C' && RX_MSG[1] == 'W')	// CW = Envio de número do cartão wireless
	{
		for(i=0; i<6; i++)
		{
			CARD_NUMBER[i] = RX_MSG[2+i] - '0';
			CARD_PASSWORD[i] = RX_MSG[8+i] - '0';
		}
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
	
	COUNT_40S = 0;
	
	USART_Receive_String();
	USART_Transmit_String(4);
}
