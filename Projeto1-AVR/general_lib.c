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
		0 -> pino de input do microcontrolador
		1 -> pino de output do microcontrolador	*/
	DDRB = 0xFF; // 1111 111
	DDRC = 0x3F; //xx11 1111 -> PC0, PC1, PC2 e PC3 as output e PC4 (led fora do ar) e PC5 (pag pendente) como output
	DDRD = 0x00; //x000 xxxx -> PD4, PD5 e PD6 as input
	
	CHECK_PAGAMENTO_AVISTA = 0;
	CHECK_COMPRA_PARCELADA = 0;
	CHECK_ESTORNO = 0;
	CHECK_PARCELA_AGENDADA = 0;
	
	ERROR_COM=0;
	
	PORTC &= ~(1 << 4); // inicializa com o led desligado
	PORTC &= ~(1 << 5); // inicializa com o led desligado
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
	
	// inicializo parcelas
	for(int i=0; i<10; i++)
	{
		parcelas[i].enable = 0; // desabilita todas as parcelas
		parcelas[i].valor = 0;
		operador1.estornos[i] = 0;
		operador2.estornos[i] = 0;
	}
	
	// inicializa os dados de data e hora
	set_hour(8, 30, 0); // inicializa o sistema as 8h30
	set_date(10, 4, 2022); // inicializa no dia 3 de abril de 2022
	enable_GC = 0;
	COUNT = 0;
	
	CHECK_PAGAMENTO_AVISTA = 0;
	CHECK_COMPRA_PARCELADA = 0;
	CHECK_ESTORNO = 0;
	CHECK_PARCELA_AGENDADA = 0;
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
		DAY++;
	}
	
	if (MONTH == 2) //fevereiro
	{
		if(YEAR%4 == 0) // bissexto
		{
			if(DAY > 29)
			{
				DAY = 1;
				MONTH++;
			}
		}
		else if(DAY > 28)
		{
			DAY = 1;
			MONTH++;
		}
	}
	else if((MONTH==4)||(MONTH==6)||(MONTH==9)||(MONTH==11)) // se mes tem 30 dias
	{
		if(DAY > 30)
		{
			DAY = 1;
			MONTH++;
		}
	}
	else if(DAY > 31)
	{
		DAY = 1;
		MONTH++;
	}
	if (MONTH > 12)
	{
		MONTH = 1;
		YEAR++;
	}
}

void display_time()
{
	LCD_clear();
	char h_d, h_u, m_d, m_u, s_d, s_u;
	char day_d, day_u, month_d, month_u, y_m, y_c, y_d, y_u;
	
	day_d = ((DAY/10)%10) + '0';
	day_u = (DAY%10) + '0';
	
	month_d = ((MONTH/10)%10) + '0';
	month_u = (MONTH%10) + '0';
	
	y_m = ((YEAR/1000)%10) + '0';
	y_c = ((YEAR/100)%10) + '0';
	y_d = ((YEAR/10)%10) + '0';
	y_u = (YEAR%10) + '0';
	
	h_d = ((HOUR/10)%10) + '0';
	h_u = (HOUR%10) + '0';
	
	m_d = ((MINUTE/10)%10) + '0';
	m_u = (MINUTE%10) + '0';
	
	s_d = ((SECOND/10)%10) + '0';
	s_u = (SECOND%10) + '0';
	
	sendString("Data ");
	sendChar(day_d);
	sendChar(day_u);
	sendChar('/');
	sendChar(month_d);
	sendChar(month_u);
	sendChar('/');
	sendChar(y_m);
	sendChar(y_c);
	sendChar(y_d);
	sendChar(y_u);
	send_command(0xC0,0);
	sendString("Hora  ");
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
	if(enable_GC == 1) // se o contador global estiver habilitado incrementa
	{
		COUNT++;
	}
	if(enable_40S) // se o contador de 40s estiver habilitado, incremena
	{
		COUNT_40S++;
	}
	if(COUNT_40S == 40 || COUNT_40S == 80)
	{
		if (CHECK_PAGAMENTO_AVISTA == 1)
		{
			USART_Transmit_String(1);
		}
		else if (CHECK_COMPRA_PARCELADA == 1)
		{
			USART_Transmit_String(2);
		}
		else if (CHECK_ESTORNO == 1)
		{
			USART_Transmit_String(3);
		}
		else if (CHECK_PARCELA_AGENDADA == 1)
		{
			USART_Transmit_String(4);
		}
	}
	if(COUNT_40S == 120)
	{
		PORTC |= (1 << 4); //led fora do ar ON
		
		ERROR_COM = 1;
		
		COUNT_40S = 0;
		enable_40S = 0;
	}
}

