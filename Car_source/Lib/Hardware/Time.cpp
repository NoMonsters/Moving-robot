#include "Time.h"

extern volatile uint32_t OVF_counter;

uint32_t Get_time()
{
	uint8_t Current_counter_value = TCNT2;
	uint32_t Current_Time = OVF_counter*Time_of_one_OVF+Current_counter_value*static_cast<uint32_t>(Time_of_one_tic);
	
	return Current_Time;
}
