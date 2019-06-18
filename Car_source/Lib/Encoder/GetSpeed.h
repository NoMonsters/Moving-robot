#ifndef GETSPEED_H_
#define GETSPEED_H_

#include "../Hardware/Time.h"

const uint8_t How_many_speed_slots = 5;
const uint32_t Zero_speed_timeOut = 800000;

void Get_speed_left(float &);
void Get_speed_right(float &);
void Does_speed_zero(uint32_t, uint32_t, float &, float &);


#endif