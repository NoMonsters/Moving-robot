#include "math.h"

double getOmegaAim(double lambda_aim, double phi_aim, double lambda_tek, double phi_tek, double hdg, double abs_speed)
{
	double x_aim = 0, y_aim = 0; //���������� ���� � ����� ��������������� ��
	double speed_x = 0, speed_y = 0; //������ �������� � ��������������� ��
	double abs_aim = 0; // ������ ������� ����������� �� ����
	double vect_prod = 0; // ���������� ��� �������� ������������ ���������� ���������� ������������
	double alpha = 0; // ���� �� ����

	const long Earth_Radius = 6371000; //������ �����
	const float pi = 3.14159265;

	x_aim = ((lambda_aim - lambda_tek)*pi / 180)*Earth_Radius;
	y_aim = ((phi_aim - phi_tek)*pi / 180)*Earth_Radius; //������� � ��������������� ����������, ������������� �� � ��������� �����

	hdg = hdg * pi / 180; //������� � �������

	// ���������� ������� ������� �������� � ��������������� ��
	if (hdg <= pi / 2)
	{
		speed_x = abs_speed * sin(hdg);
		speed_y = abs_speed * cos(hdg);
	}
	else
		if ((hdg > pi / 2) && (hdg <= pi))
		{
			speed_x = abs_speed * sin(pi - hdg);
			speed_y = -abs_speed * cos(pi - hdg);
		}
		else
			if ((hdg > pi) && (hdg <= 3 * pi / 2))
			{
				speed_x = -abs_speed * sin(hdg - pi);
				speed_y = -abs_speed * cos(hdg - pi);
			}
			else
				if ((hdg > 3 * pi / 2) && (hdg <= 2 * pi))
				{
					speed_x = -abs_speed * sin(2 * pi - hdg);
					speed_y = abs_speed * cos(2 * pi - hdg);
				}

	abs_aim = sqrt(pow(x_aim, 2) + pow(y_aim, 2)); //������ ������ ������� ����������� �� ����

	vect_prod = speed_x * y_aim - speed_y * x_aim; //������������ ���������� ���������� ������������ ������� �� ���� � ��������

	alpha = asin(vect_prod / (abs_speed*abs_aim)); //������ ���� �� ���� ����� ������ ���������� ������������

	 //�� �������� ������� �� -pi/2 �� pi/2 ��� �������� ���� ���� ��� � ����� �����������
	if ((phi_aim < phi_tek) && (lambda_aim > lambda_tek))
	{
		alpha = -alpha - pi;
	}

	if ((phi_aim < phi_tek) && (lambda_aim < lambda_tek))
	{
		alpha = pi - alpha;
	}
	 
	return alpha;
}