#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

//VARIASVEIS GLOBAIS
char USER_PASSWORD[3], CARD_PASSWORD[6];
int STATE;

typedef struct{
	float saldo;
	//float estornos[10]={0};
	int total_estornos;
}OPERADOR;

//FUN합ES LCD
void LCD_init();
void send_command(char c, char rs);
void sendString(char* str);
void sendChar(char c);
void LCD_clear();
void LCD_print2lines(char *line1, char *line2);
void delay_lcd(); //TIM0
void delay_clear(); ////TIM0

//FUNCOES TECLADO
char keyboard_input();
char debouncing(char TECLA);
void delay_debouncing();

//FUN합ES PAYMENT
int maquina_on_off(char mode);
int read_user_password();
void read_4pass();
int select_mode();
void read_price();
void avista_mode();

//FUN합ES GENERAL
void PINS_init();
void VAR_init();

//FUN합ES TIMER
void delay_3s();
void delay_4s();