#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

typedef struct{
	float saldo;
	//float estornos[10]={0};
	int total_estornos;
	int enable;
} OPERADOR;

//VARIAVEIS GLOBAIS
OPERADOR operador1 ,operador2; // struct com os dados de cada operador
char USER_PASSWORD[3], CARD_PASSWORD[6]; // vetores com as senhas de user e do cartão
int STATE; // estado do sistema

int OP; //1 - operador 1, 2 - operador 2, 3 - administrador
int OP_PAGAMENTO; //1 - débito 1, 2 - crédito, se for = 0, é estorno
float VALOR_OPERACAO; //valor da operação (pagamento ou estorno)

//GLOBAIS DE CLOCK E TEMPOS
int HOUR, MINUTE;
int COUNT_3S, enable_3S, COUNT_4S, enable_4S;

//FUNÇÕES LCD
void LCD_init();
void send_command(char c, char rs);
void sendString(char* str);
void sendChar(char c);
void LCD_clear();
void LCD_print2lines(char *line1, char *line2);

//FUNCOES TECLADO
char keyboard_input();
char debouncing(char TECLA);

//FUNÇÕES PAYMENT
int maquina_on_off(char mode);
int read_user_password();
void read_4pass(int n);
void read_pass(int n);
int select_mode();
void debit_or_credit ();
void insere_cartao();
void numero_parcelas();
void senha_cartao();
void read_price();
void aprazo_mode();
void avista_mode();

//FUNÇÕES GENERAL
void MICPAY_init();
void set_hour(int in_hour, int in_minute);
void VAR_init();
void display_time();
void update_clock();

//FUNÇÕES TIMER
void delay_3s(); //TIM1
void delay_4s(); //TIM1
void delay_debouncing(); //TIM0
void delay_lcd(); //TIM0
void delay_clear(); //TIM0
ISR(TIMER1_OVF_vect); //interrupt TIM1
int press_confirm();
int press_cancel();

//FUNÇÕES SERIAL
void Uart_init();
unsigned char USART_Receive();
void USART_Transmit_String (int code);
ISR(USART_RX_vect);