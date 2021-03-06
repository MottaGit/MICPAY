#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

typedef struct{
	float saldo;
	float estornos[10];
	int total_estornos;
	int enable;
} OPERADOR;

typedef struct{
	int enable;
	int ano;
	int mes;
	int dia;
	float valor;
	char CARD_N[6];
} PARCELA;

//VARIAVEIS GLOBAIS
OPERADOR operador1 ,operador2; // struct com os dados de cada operador
PARCELA parcelas[10];
char USER_PASSWORD[4],  CARD_NUMBER[6], CARD_PASSWORD[6]; // vetores com as senhas de user e do cart?o
int STATE; // estado do sistema
char CHECK_PAGAMENTO_AVISTA, CHECK_COMPRA_PARCELADA, CHECK_ESTORNO, CHECK_PARCELA_AGENDADA, ERROR_COM;

int NUM_PARCELAS; // Numero de parcelas do pagamento
int OP; //1 - operador 1, 2 - operador 2, 3 - administrador
int OP_PAGAMENTO; //1 - d?bito, 2 - cr?dito, 0 - estorno
float PAYMENT_VALUE; //valor da opera??o (pagamento ou estorno)

//GLOBAIS DE CLOCK E TEMPOS
int YEAR, MONTH, DAY, HOUR, MINUTE, SECOND;
int COUNT, enable_GC, COUNT_40S, enable_40S;

//FUN??ES LCD
void LCD_init();
void send_command(char c, char rs);
void sendString(char* str);
void sendChar(char c);
void LCD_clear();
void LCD_print2lines(char *line1, char *line2);

//FUNCOES TECLADO
char keyboard_input();
char debouncing(char TECLA);

//FUN??ES PAYMENT
int maquina_on_off(char mode);
int read_user_password();
void read_4pass(int n);
int read_pass(int n);
int select_mode();
void debit_or_credit ();
int confirma_senha_op();
int insere_cartao();
int compare_pass(int n);
void numero_parcelas();
int senha_cartao();
void read_price();
void aprazo_mode();
void avista_mode();
int read_mode_adm();
int en_dis_op();
int altera_hora();
int altera_data();
void verifica_pendencia();
void imprime_response_serial();
int relatorios();

//FUN??ES GENERAL
void MICPAY_init();
void set_hour(int in_hour, int in_minute, int in_second);
void set_date(int in_day, int in_month, int in_year);
void VAR_init();
void display_time();
void update_clock();
void global_counters();
void agenda_parcelas();
int menu_pendencias();

//FUN??ES TIMER
void delay_3s(); //TIM1
void delay_4s(); //TIM1
void delay_debouncing(); //TIM0
void delay_lcd(); //TIM0
void delay_clear(); //TIM0
ISR(TIMER1_OVF_vect); //interrupt TIM1
int press_confirm();
int press_cancel();

//FUN??ES SERIAL
void Uart_init();
unsigned char USART_Receive();
void USART_Transmit_String (int code);
int USART_Receive_String();
ISR(USART_RX_vect);
void check_receive();