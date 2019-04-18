#include "stdafx.h"
#include "GPSParser.h"


GPSParser::GPSParser()
{

}

GPSParser::GPSParser(char type)
{
	
	if (type == 'R')
	{
		printf("Работаем по мнемонике RMC\n");
	}
}

void GPSParser::parse_data(char *data, int size)
{
	int count = get_feild_count(data, size);

}

char* GPSParser::get_field(int begin,int end)
{
	char result[] = "dd";
	return result;

}

int GPSParser::get_feild_count(char *data, int size)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (data[i] == ',')
			count++;
	}
	printf("%d\n", count);
	return count;
}

GPSParser::~GPSParser()
{
}
