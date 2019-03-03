#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;

class GPSData
{
public:
	string type; //формат
	float latitude; //широта
	float longitude; //долгота
	float latitudeDeg; //широта в градусах
	float longitudeDeg; //долгота в градусах
	float time; //время UTC определения координат
	string status;
	string indicator;
	string statusDescription;
	GPSData(string type);
	~GPSData();
	string getStatusDesciptionGLL(string status, string indicator);
	vector<string> parseData(char line[]);
};
