#include "GetSpeed.h"

extern volatile uint8_t Left_Encoder_counter;	//количество срабатываний энкодера. Левое колесо
extern volatile uint8_t Right_Encoder_counter;	//количество срабатываний энкодера. Правое колесо

void Get_speed_left(float &Current_speed_left)
{
	static uint32_t Prev_call_time_l = 0;
	uint32_t Time_now = Get_time();
	
	Current_speed_left = 15000000.0/(Time_now - Prev_call_time_l);
	Prev_call_time_l = Time_now;

	Left_Encoder_counter = 0;
}
void Get_speed_right(float &Current_speed_right)
{
	static uint32_t Prev_call_time_r = 0;
	uint32_t Time_now = Get_time();
	
	Current_speed_right = 15000000.0/(Time_now - Prev_call_time_r);
	Prev_call_time_r = Time_now;

	Right_Encoder_counter = 0;
}
void Does_speed_zero(uint32_t Last_speed_check_left, uint32_t Last_speed_check_right, float &Current_speed_left , float &Current_speed_right)
{
	uint32_t Current_time = Get_time();
	
	if(Current_time - Last_speed_check_left >= Zero_speed_timeOut)
	Current_speed_left = 0;
	if(Current_time - Last_speed_check_right >= Zero_speed_timeOut)
	Current_speed_right = 0;
}