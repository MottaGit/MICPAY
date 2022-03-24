#include "header.h"
#include <avr/io.h>

void delay_3s()
{
	// T_Timer = 1/16MHz = 62.5ns
	// Prescalar: 1024 -> T_Timer = 62.5ns * 1024 = 64us
	// N_contagens = 3s/64us = 46875
	TCNT1 = 18661; // 65536 - 46875 = 18661
	while ((TIFR1 & (1 << 0)) == 0); //aguarda flag
	TIFR1 |= (1 << 0); //limpa flag
}

void delay_4s()
{
	// T_Timer = 1/16MHz = 62.5ns
	// Prescalar: 1024 -> T_Timer = 62.5ns * 1024 = 64us
	// N_contagens = 4s/64us = 62500
	TCNT1 = 3036; // 65536 - 62500 = 3036
	while ((TIFR1 & (1 << 0)) == 0); //aguarda flag
	TIFR1 |= (1 << 0); //limpa flag
}