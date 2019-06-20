/* 
* Filter.h
*
* Created: 20.06.2019 22:37:43
* Author: Pugach
*/


#ifndef __FILTER_H__
#define __FILTER_H__


class Filter
{
//variables
public:
protected:
private:
float kp;
float ki;
float error;
float filtered_value;


//functions
public:
	Filter(float kp, float ki);
	~Filter();
	float Correct(float value);
	
protected:
private:

}; //Filter

#endif //__FILTER_H__
