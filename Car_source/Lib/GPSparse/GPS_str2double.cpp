#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

int p = 10;
int null_flag = 0;;

double GPS_str2double(char* strL, char* strH)
{
	p = 10;
	null_flag = 0;
	while (atoi(strL) / p > 1)
	{
		p = p * 10;
	}
	for (int i = 0; i < strlen(strL); i++)
	{
		if (strL[i] != '0')
		{
			null_flag = 1;
		}
		if ((strL[i] == '0') && (null_flag == 0))
		{
			p = p * 10;
		}
	}
	return atof(strL) / p + atof(strH);
}