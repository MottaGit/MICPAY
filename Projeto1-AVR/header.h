/*
 * header.h
 */ 
//VARIASVEIS GLOBAIS
//char STATE=0, USER_PASSWORD[4], CARD_PASSWORD[6];

//FUN합ES LCD
void LCD_init();
void send_command(char c, char rs);
void sendString(char* str);
void sendChar(char c);
void LCD_clear();
void LCD_print2lines(char *line1, char *line2);
void delay_lcd();
void delay_clear();

//FUNCOES TECLADO
char keyboard_input();
char debouncing(char TECLA);
void delay_debouncing();

//FUN합ES PAYMENT
int maquina_on_off(char mode);
int read_user_password();

//FUN합ES GENERAL
void PINS_init();
void displays_menu();

//FUN합ES TIMER
void delay_3s();