#include "GPS_str2double.h"


double GPS_str2double(char* strL, char* strH)
{
	double p = 10.0;
	int null_flag = 0;
	
	while (atof(strL) / p > 1)
	{
		p = p * 10.0;
	}
	for (int i = 0; i < 10; i++)
	{
		if (strL[i] != '0')
		{
			null_flag = 1;
		}
		if ((strL[i] == '0') && (null_flag == 0))
		{
			p = p * 10.0;
		}
	}
	return atof(strL) / p + atof(strH);
}