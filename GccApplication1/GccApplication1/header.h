/*
 * header.h
 *
 * Created: 22/03/2022 23:28:41
 *  Author: vini2
 */ 
//FUNÇÕES DE ATRASO
void delay_lcd();
void delay_debouncing();
void delay_clear();

//FUNÇÕES LCD
void LCD_init();
void send_command(char c, char rs);
void sendString(char* str);
void LCD_clear();