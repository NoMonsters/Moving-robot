#include "pch.h"
#include "math.h"

double getOmegaAim(double lambda_aim, double phi_aim, double lambda_tek, double phi_tek, double hdg, double abs_speed)
{
	double x_aim, y_aim; //���������� ���� � ����� ��������������� ��
	double speed_x, speed_y; //������ �������� � ��������������� ��
	double abs_aim; // ������ ������� ����������� �� ����
	double vect_prod; // ���������� ��� �������� ������������ ���������� ���������� ������������
	double alpha; // ���� �� ����

	const long Earth_Radius = 6371000; //������ �����
	const float pi = 3.14159265;

	x_aim = ((lambda_aim - lambda_tek)*pi / 180)*Earth_Radius;
	y_aim = ((phi_aim - phi_tek)*pi / 180)*Earth_Radius; //������� � ��������������� ����������, ������������� �� � ��������� �����

	speed_x = abs_speed * sin(hdg);
	speed_y = abs_speed * cos(hdg); // ���������� ������� ������� �������� � ��������������� ��

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

	alpha = alpha * 180 / pi; //������� � ������� (��� ��������� � ��������)
	return alpha;
}