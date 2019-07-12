/* 
* Filter.cpp
*
* Created: 20.06.2019 22:37:43
* Author: Pugach
*/


#include "Filter.h"

// default constructor
Filter::Filter(float kp, float ki)
{
	this->kp=kp;
	this->ki=ki;
	this->filtered_value=0;
	this->error=0;
} //Filter

// default destructor
Filter::~Filter()
{
} //~Filter

float Filter::Correct(float value)
{
	float error_temp = value - filtered_value;
	
	filtered_value = filtered_value + kp*(error_temp-error)+ki*error_temp;
	error = error_temp;

	return filtered_value;
}
