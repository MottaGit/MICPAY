#include "header.h"

#define F_CPU 16000000UL
#define USART_BAUDRATE 9600
#define UBRR_VALUE ((F_CPU/16/USART_BAUDRATE)-1)

char RX_Buffer[9];

//VAR GLOBAIS -> algumas precisam ser criadas e adaptadas ao codigo.
char RX_MSG[9];
char CARD_NUMBER[6];
float PAYMENT_VALUE;
int NUM_PARCELAS;

void Uart_init()
{
	UBRR0L = UBRR_VALUE;
	UBRR0H = UBRR_VALUE >> 8;
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); // habilita transmissão/recepção e interrupção de recepção
	UCSR0C = 0x06; // Set: 8data, 1stop bit
	
}

unsigned char USART_Receive()
{
	while(!(UCSR0A) && (1<<RXC0));
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
			TX_MSG[2+i] = CARD_NUMBER[i];
			TX_MSG[8+i] = CARD_PASSWORD[i];
		}
		
		TX_MSG[14] = ((int)(PAYMENT_VALUE*100)/1000)%10;
		TX_MSG[15] = ((int)(PAYMENT_VALUE*100)/100)%10;
		TX_MSG[16] = ((int)(PAYMENT_VALUE*100)/10)%10;
		TX_MSG[17] = ((int)PAYMENT_VALUE*100)%10;
		
		TX_MSG[18] = '\r';
		
		break;
		
		case 2:			// PAGAMENTO PARCELADO ----------------- USART_Transmit_String(2)
		TX_MSG[0] = 'V';
		TX_MSG[1] = 'P';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i];
			TX_MSG[8+i] = CARD_PASSWORD[i];
		}
		
		TX_MSG[15] = NUM_PARCELAS;
		
		TX_MSG[15] = ((int)(PAYMENT_VALUE*100)/1000)%10;
		TX_MSG[16] = ((int)(PAYMENT_VALUE*100)/100)%10;
		TX_MSG[17] = ((int)(PAYMENT_VALUE*100)/10)%10;
		TX_MSG[18] = ((int)PAYMENT_VALUE*100)%10;
		
		TX_MSG[19] = '\r';
		
		break;
		
		case 3:			// ESTORNO ------------------------------- USART_Transmit_String(3)
		TX_MSG[0] = 'E';
		TX_MSG[1] = 'V';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i];
		}
		
		TX_MSG[8] = ((int)(PAYMENT_VALUE*100)/1000)%10;
		TX_MSG[9] = ((int)(PAYMENT_VALUE*100)/100)%10;
		TX_MSG[10] = ((int)(PAYMENT_VALUE*100)/10)%10;
		TX_MSG[11] = ((int)PAYMENT_VALUE*100)%10;
		
		TX_MSG[12] = '\r';
		
		break;
		
		case 4:			// VERIFICA PAG AGENDADO ----------------- USART_Transmit_String(4)
		TX_MSG[0] = 'A';
		TX_MSG[1] = 'P';
		
		for(i=0; i<6; i++)
		{
			TX_MSG[2+i] = CARD_NUMBER[i];
		}
		
		TX_MSG[8] = ((int)(PAYMENT_VALUE*100)/1000)%10;
		TX_MSG[9] = ((int)(PAYMENT_VALUE*100)/100)%10;
		TX_MSG[10] = ((int)(PAYMENT_VALUE*100)/10)%10;
		TX_MSG[11] = ((int)PAYMENT_VALUE*100)%10;
		
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

ISR(USART_RX_vect)
{
	int i;
	
	for(i=0; i<9; i++)
	{
		RX_Buffer[i] = USART_Receive();
	}
	
	strcpy(RX_MSG, RX_Buffer);
}