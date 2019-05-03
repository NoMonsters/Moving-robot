#pragma once
class GPSParser
{
public:
	char *msg_type;
	char *timeUTC;
	char status;
	double latitude;
	char latitude_north_or_south;
	double longitude;
	char longitude_east_or_west;
	double velocity;
	double course;
	char *date;
	double magnetic_declination;
	char mag_dec_east_or_west;
	char indicator;

	

	GPSParser();
	GPSParser(char type);
	void parse_data(char * data, int size);
	char * get_field(int begin, int end);
	int get_feild_count(char * data, int size);
	~GPSParser();
};

