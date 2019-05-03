#include "UART.h"

void UART_init()
{
	uint16_t const UBRRL_value = (F_CPU/(BAUD*8))-1; //��������� �������� �������� ������������ �������� ��� �������� UBRR
	
	UBRR0L = UBRRL_value;							//������� 8 ��� UBRRL_value
	UBRR0H = (UBRRL_value>>8);						//������� 8 ��� UBRRL_value
	UCSR0A |=(1 << U2X0);							//��������� ������� ��������
	UCSR0B |=(1<<TXEN0)|(1<<RXEN0);					//��� ���������� �������� � ��� ���������� ������
	UCSR0C |=(1<< UCSZ00)|(1<< UCSZ01);				//������������ ������ 8 ��� ������
}

void UART_send_char(char value)
{
	while(!( UCSR0A & (1 << UDRE0)));   // ������� ����� ��������� ����� ��������
	UDR0 = value;						// �������� ������ � �����, �������� ��������
}

char UART_get_char()
{
	while(!( UCSR0A  & (1 << RXC0)));   // ������� ����� ��������� ����� ��������
	return UDR0;						// �������� ������ � �����, �������� ��������
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