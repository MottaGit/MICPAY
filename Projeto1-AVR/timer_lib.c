#include "header.h"

int press_confirm()
{
	char flag = 0;
	
	PORTC = 0x07; //0000 0111 -> zera porta PC3
	enable_3S = 1; // habilita contador de 3s
	
	while(((PIND & (1 << 6)) == 0) && !flag) // verifica se '#' esta pressionado ou se ja contou 3 seg
	{
		if(COUNT_3S == 3) // verifica se o contador chegou em 3
			flag = 1;
	}
	
	enable_3S = 0; // desabilita contador de 3s
	COUNT_3S = 0;  // zera contador
	
	if(!flag)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int press_cancel()
{
	char flag = 0;
	
	PORTC = 0x07; //0000 0111 -> zera porta PC3
	enable_4S = 1; // habilita contador de 4s
	
	while(((PIND & (1 << 4)) == 0) && !flag) // verifica se '*' esta pressionado ou se ja contou 4 seg
	{
		if(COUNT_4S == 4) // verifica se o contador chegou em 4
		flag = 1;
	}
	
	enable_4S = 0; // desabilita contador de 4s
	COUNT_4S = 0;  // zera contador
	
	if(!flag)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

// delay de avisos exibidos na tela.
void delay_3s() // USAR O CONTADOR DE 1 SEG
{
	// T_Timer = 1/16MHz = 62.5ns
	// Prescalar: 1024 -> T_Timer = 62.5ns * 1024 = 64us
	// N_contagens = 3s/64us = 46875
	TCCR1B = 0x5; //Prescaler 1024
	
	TCNT1 = 18661; // 65536 - 46875 = 18661
	while ((TIFR1 & (1 << 0)) == 0); //aguarda flag
	TIFR1 = (1 << 0); //limpa flag
}

//Função atraso para o debouce 1ms
//Frequência do timer: 16Mhz / 2 = 8000kHz
void delay_debouncing()
{
	// T_Timer = 1/16MHz = 62.5ns
	// Prescaler: 256-> 16MHz/256 = 62500Hz / T_Timer = 1/62500 = 16us
	//num_contagens = (1E-3)/16E-6 = 62
	TCCR0B = 0x04; // Configura preescaler de 256
	TCNT0 = 194; //256 - 62 = 194
	
	while ((TIFR0 & (1 << 0)) == 0); //aguarda flag
	TIFR0 |= (1 << 0); //limpa flag
}

void delay_lcd()
{
	TCCR0B = 0x02;				//prescaler de 8, T_timer = 8*T_CPU = 0,5us
	TCNT0 = 176;				//(40us/0,5us) = 80 contagens -> 256-80 = 176
	while(!(TIFR0 & (1<<0)));	//aguarda estouro
	TIFR0 = (1<<0);				//limpa flag de estouro
}

void delay_clear()
{
	TCCR0B = 0x04;				//prescaler de 256, T_timer = 256*T_CPU = 16us
	TCNT0 = 153;				//(1640us/16us) = 103 contagens -> 256-103 = 153
	while(!(TIFR0 & (1<<0)));	//aguarda estouro
	TIFR0 = (1<<0);				//limpa flag de estouro
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 0x0BDC;	//contagem de 1seg, para prescaler de 256 => 1min do sistema
	TIFR1 = (1 << 0);   // Limpa flag de overflow
	
	update_clock();
	
	//display_time();
	
	if(enable_3S) //se o contador de 3s estiver habilitado incrementa
	{
		COUNT_3S++;
	}
	if(enable_4S) //se o contador de 4s estiver habilitado incrementa
	{
		COUNT_4S++;
	}
}