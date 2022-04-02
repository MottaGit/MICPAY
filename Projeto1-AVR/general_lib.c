#include "header.h"

// inicializa os pinos usados no projeto
void MICPAY_init()
{
	/* configurações para modo de operação normal nos timers */
	TCCR0A = 0;
	TCCR1A = 0;
	TCCR2A = 0;
	
	// configurações iniciais para TIM1 para contagem de 1 seg = 1 min no sistema
	TCCR1B = 0x04;	// prescaler de 256 -> tempo de uma contagem é 0,0625us*256 = 16us
	TCNT1 = 0x0BDC;	// 1s/16us = 62500 contagens -> 65536-62500 = 3036
	
	// interrupções do timer
	sei();
	TIMSK1 |= (1 << 0); // interrupção flag estouro tim1
	//TIMSK2 = 0x01; // interrupção flag estouro tim2
	
	
	/*	pinos como entrada e saída:
		0 -> pino de entrada do microcontrolador
		1 -> pino de saída do microcontrolador	*/
	DDRB = 0xFF; // 1111 111
	DDRC = 0x0F; //xx00 1111 -> PC0, PC1, PC2 e PC3 as input e PC4 (led fora do ar) e PC5 (pag pendente) como output
	DDRD = 0x00; //x000 xxxx -> PD4, PD5 e PD6 as output
}

void set_hour(int in_hour, int in_minute)
{
	HOUR = in_hour;
	MINUTE = in_minute;
}

// inicializa variáveis do sistema
void VAR_init()
{
	// inicializa dados dos operadores
	operador1.saldo = 0;
	operador1.total_estornos=0;
	operador1.enable = 1;
	
	operador2.saldo = 0;
	operador2.total_estornos=0;
	operador2.enable = 1;
	
	// inicializa os dados de hora
	set_hour(8, 0);
	enable_4S = 0;
	enable_3S = 0;
	COUNT_3S = 0;
	COUNT_4S = 0;
}

void update_clock()
{
	MINUTE++;
	if (MINUTE > 59)
	{
		MINUTE = 0;
		HOUR++;
	}
	if (HOUR>23)
	{
		HOUR = 0;
	}
}

void display_time()
{
	char h_d, h_u, m_d, m_u;
	h_d = ((HOUR/10)%10) + '0';
	h_u = (HOUR%10) + '0';
	
	m_d = ((MINUTE/10)%10) + '0';
	m_u = (MINUTE%10) + '0';
	
	LCD_clear();
	sendChar(h_d);
	sendChar(h_u);
	sendChar(':');
	sendChar(m_d);
	sendChar(m_u);
}