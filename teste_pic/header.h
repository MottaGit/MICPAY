//Header com todas as inicializações de bibliotecas e funções

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//variaveis globais
char ESTADO=0, USER_PASSWORD[4], CARD_PASSWORD[6];

//funções keyboard
char keyboard_input();
char debouncing(char TECLA);
void delay_debouncing();

//funções LCD
void delay_LCD();
void delay_clear();
void send_command(char c, char val_rs);
void LCD_init();
void send_string(char* str);
void LCD_clear();

//funções timer

//funções payment