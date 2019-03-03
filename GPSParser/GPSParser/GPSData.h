#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;

class GPSData
{
public:
	string type; //������
	float latitude; //������
	float longitude; //�������
	float latitudeDeg; //������ � ��������
	float longitudeDeg; //������� � ��������
	float time; //����� UTC ����������� ���������
	string status;
	string indicator;
	string statusDescription;
	GPSData(string type);
	~GPSData();
	string getStatusDesciptionGLL(string status, string indicator);
	vector<string> parseData(char line[]);
};
