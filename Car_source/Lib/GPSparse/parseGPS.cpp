#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GPS_str2double.h"
#include "parseGPS.h"

void parseGPS(char* GPSstring, char* GPSstatus, double &GPSlatitude, char* GPS_NS, double &GPSlongitude, char* GPS_WE, double &GPS_spd, double &GPS_hdg)
{

	//время, дату и формат протокола пока не возвращаю из этой функции
	char  GPSformat[6];
	char UTC_str[9]; 
	char date[6];

	char GPSlat_strL[10];
	char GPSlat_strH[10];

	char GPSlon_strL[10];
	char GPSlon_strH[10];

	char GPS_spd_strL[10];
	char GPS_spd_strH[10];

	char GPS_hdg_strL[10];
	char GPS_hdg_strH[3];

	int k = 0; //счетчик запятых
	char buf;
	char strbuf[15];
	int j = 0; //счетчик символов от заптой до запятой
	int point_flag = 0; //флаг точки, для разделения дробных числе на 2 инта
	int q = 0; //количество символов в дробном числе до точки

	for (int i = 0; i < strlen(GPSstring); i++)
	{
		buf = GPSstring[i];

		if (buf == '$')
		{
			k = 0;
		}

		if (buf == ',')
		{
			k++;

			if (k == 2)
			{
				for (int l = 0; l < j; l++)
				{
					UTC_str[l] = strbuf[l];
				}
			}

			if (k == 3)
			{
				for (int l = 0; l < j; l++)
				{
					GPSstatus[l] = strbuf[l];
				}
			}

			if (k == 4)
			{
				for (int l = 0; l < j; l++)
				{
					if (strbuf[l] == '.')
					{
						point_flag = 1;
						q = l;
					}

					if (point_flag == 0)
					{
						GPSlat_strH[l] = strbuf[l];
					}
					else
						if (strbuf[l] != '.')
						{
							GPSlat_strL[l - q - 1] = strbuf[l];
						}
				}
			}

			if (k == 5)
			{
				for (int l = 0; l < j; l++)
				{
					GPS_NS[l] = strbuf[l];
				}
			}

			if (k == 6)
			{
				for (int l = 0; l < j; l++)
				{
					if (strbuf[l] == '.')
					{
						point_flag = 1;
						q = l;
					}

					if (point_flag == 0)
					{
						GPSlon_strH[l] = strbuf[l];

					}
					else
						if (strbuf[l] != '.')
						{
							GPSlon_strL[l - q - 1] = strbuf[l];
						}
				}
			}

			if (k == 7)
			{
				for (int l = 0; l < j; l++)
				{
					GPS_WE[l] = strbuf[l];
				}
			}

			if (k == 8)
			{
				for (int l = 0; l < j; l++)
				{
					if (strbuf[l] == '.')
					{
						point_flag = 1;
						q = l;
					}

					if (point_flag == 0)
					{
						GPS_spd_strH[l] = strbuf[l];

					}
					else
						if (strbuf[l] != '.')
						{
							GPS_spd_strL[l - q - 1] = strbuf[l];
						}
				}
			}

			if (k == 9)
			{
				for (int l = 0; l < j; l++)
				{
					if (strbuf[l] == '.')
					{
						point_flag = 1;
						q = l;
					}

					if (point_flag == 0)
					{
						GPS_hdg_strH[l] = strbuf[l];

					}
					else
						if (strbuf[l] != '.')
						{
							GPS_hdg_strL[l - q - 1] = strbuf[l];
						}
				}
			}

			j = 0;
			q = 0;
			point_flag = 0;

		}
		else
		{
			strbuf[j] = GPSstring[i];
			j++;
		}
	}

	GPSlatitude = GPS_str2double(GPSlat_strL, GPSlat_strH);
	GPSlongitude = GPS_str2double(GPSlon_strL, GPSlon_strH);
	GPS_spd = GPS_str2double(GPS_spd_strL, GPS_spd_strH);
	GPS_hdg = GPS_str2double(GPS_hdg_strL, GPS_hdg_strH);
}
