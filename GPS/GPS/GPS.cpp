// GPS.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "GPSParser.h"
#include <locale.h>


int main()
{
	char *locale = setlocale(LC_ALL, "");
	char data[] = "$GNRMC,073121.00,A,5544.7049,N,03743.7405,E,000.02626,301.4,190312,,,A*47";
	int size = sizeof(data) / sizeof(char);
	GPSParser parser = GPSParser('R');
	parser.parse_data(data, size);
	getchar();
    return 0;
}



