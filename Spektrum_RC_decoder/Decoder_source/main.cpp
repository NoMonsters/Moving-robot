/*
 * Decoder.cpp
 *
 * Created: 11.03.2019 22:30:28
 * Author : mlx-0
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "Lib/UART/UART.h"
//*******************глобальные переменные*******************

volatile uint16_t Call_UART = 0;
volatile uint32_t OVF_counter = 0;			//количество переполнений таймера TIM2
volatile uint8_t Edge_counter = 0;
volatile uint32_t Time_prev = 0;
volatile uint32_t Time_now = 0;
const uint8_t Time_of_one_tic = 2;
const uint16_t Time_of_one_OVF = 255*Time_of_one_tic;
const uint16_t How_often_call_UART = 250;
//***********************************************************
uint32_t Get_time()
{
	uint8_t Current_counter_value = TCNT2;
	uint32_t Current_Time = OVF_counter*Time_of_one_OVF+Current_counter_value*static_cast<uint32_t>(Time_of_one_tic);
	
	return Current_Time;
}

void Timer_init()
{
	TCCR2B&=~(1<<CS22);
	TCCR2B|=(1<<CS21)|(1<<CS20);	//предделитель 32
	TIMSK2|=(1<<TOIE2);				//устанавливаем прерывания по переполнению
}

void Reciver_interrupts_init()
{
	DDRD&=~(1<<DDD6);
	DDRD&=~(1<<DDD7);
	PCICR |= (1<<PCIE2);
	PCMSK2 |= (1<<PCINT6)|(1<<PCINT7);
}
void Read_RC(uint16_t &Thr, uint16_t &Rud)
{
	static uint16_t Pulse_width = 0;
	static bool ch1_or_ch2 = 0;
	
	if(Edge_counter >= 2)
	{
		Pulse_width = Time_prev - Time_now;
		Edge_counter = 0;
		ch1_or_ch2 = !ch1_or_ch2;
		TCNT2 = 0;
		OVF_counter = 0;
	}
	if(ch1_or_ch2)
		Rud = Pulse_width;
	else
		Thr = Pulse_width;
}
ISR(PCINT2_vect)
{
	if(Edge_counter == 1)
		Time_prev = Get_time();
	else
		Time_now = Get_time();
	Edge_counter++;
}
ISR(TIMER2_OVF_vect)
{
	OVF_counter++;
	Call_UART++;
}

int main(void)
{
	uint16_t Thr = 0, Rud = 0;
	
	UART_init();
	Timer_init();
	Reciver_interrupts_init();
	sei();
	
    while (1) 
    {
		Read_RC(Thr, Rud);
		if(Call_UART == How_often_call_UART)
		{
			UART_send_Str(Thr);
			UART_send_char(' ');
			UART_send_Str(Rud);
			UART_send_char('\n');
			Call_UART = 0;			
		}

    }
}

