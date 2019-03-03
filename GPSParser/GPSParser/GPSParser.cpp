// GPSParser.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include "GPSData.h"

char string1[] =
"A string\tof ,,tokens\nand some  more tokens";
char string2[] =
"Another string\n\tparsed at the same time.";
char seps[] = " ,\t\n";
char *token1 = NULL;
char *token2 = NULL;
char *next_token1 = NULL;
char *next_token2 = NULL;


int main()
{

	setlocale(LC_ALL, "Russian"); //Чтобы отображалтсь русские буквы в консоли
	GPSData gpsData = GPSData("GLL");
	char line[] = "$GNGLL,,,,,000007.00,V,N*53";
	vector<string> v = gpsData.parseData(line);

	for (string n : v)
		std::cout << n << "\n";  // 6  2  3  4  5

	
    /*std::cout << gpsData.getStatusDesciptionGLL("A", "A") << "\n"; 
	std::cout << gpsData.getStatusDesciptionGLL("A", "D") << "\n"; 
	std::cout << gpsData.getStatusDesciptionGLL("A", "E") << "\n"; 
	std::cout << gpsData.getStatusDesciptionGLL("A", "M") << "\n"; 
	std::cout << gpsData.getStatusDesciptionGLL("A", "N") << "\n"; 
	std::cout << gpsData.getStatusDesciptionGLL("V", "N") << "\n"; 
	std::cout << gpsData.getStatusDesciptionGLL("V", "") << "\n"; */

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
