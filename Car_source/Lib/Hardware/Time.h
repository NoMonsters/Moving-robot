#ifndef TIME_H_
#define TIME_H_

#include <avr/io.h>

const uint16_t TIM2_prescaler = 256;
const uint8_t Time_of_one_tic = 16;
const uint16_t Time_of_one_OVF = 255*Time_of_one_tic;

uint32_t Get_time();

#endif