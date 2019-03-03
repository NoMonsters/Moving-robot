#include "pch.h"
#include "GPSData.h"


GPSData::GPSData(string type)
{
	GPSData::type = type;
}

GPSData::~GPSData()
{
}

string GPSData::getStatusDesciptionGLL(string status, string indicator)
{
	string result = "��������� �������� �������� �������";

	if (status == "A")
	{
		result = "���������� ����������";

		if (indicator == "A")
			result += ": ���������� �����";

		if (indicator == "D")
			result += ": ���������������-������� �����, ������� SBAS";

		if (indicator == "E")
			result += ": ������������� ���������";

		if (indicator == "M")
			result += ": ����� ������������� ���������";
	}
	else
	{
		if (status == "V")
		{
			result = "���������� ������������";

			if (indicator == "N")
				result += ": ������ ������������";
		}
	}

	return result;
}

vector<string>  GPSData::parseData(char line[])
{
	int count = 0;
	char seps[] = ",";
	char *token = NULL;
	char *next_token = NULL;
	vector<string> gpsData(8);

	token = strtok_s(line, seps, &next_token);

	// While there are tokens in "string1" or "string2"
	while ((token != NULL))
	{
		// Get next token:
		if (token != NULL)
		{
			//printf(" %s\n", token);
			gpsData[count] = token;
			token = strtok_s(NULL, seps, &next_token);
			count++;
		}
	}
	return gpsData;
}
