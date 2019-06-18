#ifndef UART_H_					/* Define library H file if not defined */
#define UART_H_

#define F_CPU 16000000UL				/* Define CPU clock Frequency e.g. here its 8MHz */
#define BAUD 38400L						// �������� ������ �������
#define DEC 10							//���������� �������

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

void UART_init();
void UART_send_char(char);
void UART_send_Str(char*);
void UART_send_Str(int);
char UART_get_char();
<<<<<<< HEAD
=======

>>>>>>> NAV

#endif