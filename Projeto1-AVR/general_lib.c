#include "header.h"

// inicializa os pinos usados no projeto
void PINS_init()
{
	/*	definir pinos de entrada e saída das portas:
		0 -> pino de entrada do microcontrolador
		1 -> pino de saída do microcontrolador	*/
	
	//pinos para o LCD
	DDRB = 0xFF; // 1111 111
	
	//pinos para o keyboard
	DDRC = 0x0F; //xx00 1111 -> PC0, PC1, PC2 e PC3 as input e PC4 e PC5 como output
	// PC4 -> Led fora do ar
	// PC5 -> Led pagamento pendente
	DDRD = 0x00; //x000 xxxx -> PD4, PD5 e PD6 as output
}

// inicializa variáveis com informações dos operadores
void VAR_init()
{
	OPERADOR operador1;
	OPERADOR operador2;
	
	operador1.saldo = 0;
	operador1.total_estornos=0;
	operador2.saldo = 0;
	operador2.total_estornos=0;
}