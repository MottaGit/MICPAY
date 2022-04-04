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

void set_hour(int in_hour, int in_minute, int in_second)
{
	HOUR = in_hour;
	MINUTE = in_minute;
	SECOND = in_second;
}

void set_date(int in_day, int in_month, int in_year)
{
	DAY = in_day;
	MONTH = in_month;
	YEAR = in_year;
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
	
	// inicializa os dados de data e hora
	set_hour(8, 30, 0); // inicializa o sistema as 8h30
	set_date(4, 3, 2022); // inicializa no dia 4 de março de 2022
	enable_GC = 0;
	COUNT = 0;
}

void update_clock()
{
	SECOND++;
	if (SECOND > 59)
	{
		SECOND = 0;
		MINUTE++;
	}
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
	LCD_clear();
	char h_d, h_u, m_d, m_u, s_d, s_u;
	h_d = ((HOUR/10)%10) + '0';
	h_u = (HOUR%10) + '0';
	
	m_d = ((MINUTE/10)%10) + '0';
	m_u = (MINUTE%10) + '0';
	
	s_d = ((SECOND/10)%10) + '0';
	s_u = (SECOND%10) + '0';
	
	sendChar(h_d);
	sendChar(h_u);
	sendChar(':');
	sendChar(m_d);
	sendChar(m_u);
	sendChar(':');
	sendChar(s_d);
	sendChar(s_u);
}

void global_counters()
{
	if(enable_GC) // se o contador global estiver habilitado incrementa
	{
		COUNT++;
	}
	if(enable_40S) // se o contador de 40s estiver habilitado, incremena
	{
		COUNT_40S++;
	}
	if(COUNT_40S == 40 || COUNT_40S == 80)
	{
		//manda comando serial novamentee
	}
	if(COUNT_40S == 120)
	{
		//led fora do ar ON
		COUNT_40S = 0;
		enable_40S = 0;
	}
}