#include "UART.h"

void UART_init()
{
	uint16_t const UBRRL_value = (F_CPU/(BAUD*8))-1; //Согластно заданной скорости подсчитываем значение для регистра UBRR
	
	UBRR0L = UBRRL_value;							//Младшие 8 бит UBRRL_value
	UBRR0H = (UBRRL_value>>8);						//Старшие 8 бит UBRRL_value
	UCSR0A |=(1 << U2X0);							//Установка двойной скорости
	UCSR0B |=(1<<TXEN0)|(1<<RXEN0);					//Бит разрешения передачи и бит разрешения приема
	UCSR0C |=(1<< UCSZ00)|(1<< UCSZ01);				//Устанавливем формат 8 бит данных
}

void UART_send_char(char value)
{
	while(!( UCSR0A & (1 << UDRE0)));   // Ожидаем когда очистится буфер передачи
	UDR0 = value;						// Помещаем данные в буфер, начинаем передачу
}

char UART_get_char()
{
	while(!( UCSR0A  & (1 << RXC0)));   // Ожидаем когда очистится буфер передачи
	return UDR0;						// Помещаем данные в буфер, начинаем передачу
}


void UART_send_Str(char *str)
{
	uint8_t i = 0;
	
	while(str[i] != '\0')
	{
		UART_send_char(str[i]);
		i++;
	}
}

void UART_send_Str(int value)
{
	uint8_t i = 0;
	char str[10];
	
	itoa(value,str,DEC);
	while(str[i] != '\0')
	{
		UART_send_char(str[i]);
		i++;
	}
}