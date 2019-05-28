#ifndef HARDWARESETUP_H_
#define HARDWARESETUP_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void Encoder_interrupts_init();
void Motor_diraction_init();
void PWN_for_wheels_init();
void Timer_for_wheels_init();

#endif