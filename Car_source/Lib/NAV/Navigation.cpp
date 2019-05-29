#include "Navigation.h"

void Turn_control(float OMEGA, float Velocity, float Current_OmegaZ, float *Omega_LR_required, bool straight)
{
	static float const Wheel_radius = 0.03375; //from prev function
	float omega_const_1 = 0, omega_const_2 = 0;
	float Radius = Velocity/OMEGA, omega_additional = 0;
	//char Float_to_char_buffer1[5], Float_to_char_buffer2[5];
	
	//Calc_OmegaLeftReq_OmegaRightReq_for_turn(Radius, omega_const_1, omega_const_2, OMEGA, straight);
	
	omega_const_1 = 2*Velocity/Wheel_radius;
	omega_const_2 = 2*Velocity/Wheel_radius;
	
	omega_additional = Calc_Omega_additional_using_gyro(Current_OmegaZ, OMEGA, 10.0, 0.1); //10, 0.1
	
	//dtostrf(omega_additional, 5, 2, Float_to_char_buffer1);
	//dtostrf(Current_OmegaZ, 5, 2, Float_to_char_buffer2);
	//sprintf(UART_buf, "omega_ad = %s, current = %s \n", Float_to_char_buffer1, Float_to_char_buffer2);

	Omega_LR_required[0] = 9.55*(omega_const_1 - omega_additional/2); //-9.55*omega_additional/2;
	Omega_LR_required[1] = 9.55*(omega_const_2 + omega_additional/2); //9.55*omega_additional/2;
}
double getOmegaAim(double* lambda_aim, double* phi_aim, double lambda_tek, double phi_tek, double hdg, double abs_speed)
{
	double x_aim = 0, y_aim = 0; //���������� ���� � ����� ��������������� ��
	double speed_x = 0, speed_y = 0; //������ �������� � ��������������� ��
	double abs_aim = 0; // ������ ������� ����������� �� ����
	double vect_prod = 0; // ���������� ��� �������� ������������ ���������� ���������� ������������
	double scalar_prod; //��������� ������������ ��������
	double alpha = 0; // ���� �� ����
	static int k = 0; // ����� �����, � ������� ����
	double eps = 1.0; // ������ ����������� ����� (��������)

	const long Earth_Radius = 6371000; //������ �����
	const float pi = 3.14159265;


	x_aim = ((lambda_aim[k] - lambda_tek)*pi / 180)*Earth_Radius;
	y_aim = ((phi_aim[k] - phi_tek)*pi / 180)*Earth_Radius; //������� � ��������������� ����������, ������������� �� � ��������� �����
	
	abs_aim = sqrt(pow(x_aim, 2) + pow(y_aim, 2)); //������ ������ ������� ����������� �� ����
	

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

	vect_prod = speed_x * y_aim - speed_y * x_aim; //������������ ���������� ���������� ������������ ������� �� ���� � ��������
	scalar_prod = speed_x * x_aim + speed_y * y_aim; // ��������� ������������ �������� � ��������� �������� ���������

	//������ ���� �� ���� ����� ������ ���������� ������������, ��� ������������ ����������� ������������ �������� ��� sin � cos (��������� � ��������� ��)
	if ((vect_prod>0) && (scalar_prod>0))
	{
		alpha = asin(vect_prod / (abs_speed * abs_aim));
	}
	else
		if ((vect_prod > 0) && (scalar_prod < 0))
		{
			alpha = pi - asin(vect_prod / (abs_speed * abs_aim));
		}
		else
			if ((vect_prod < 0) && (scalar_prod < 0))
			{
				alpha = -pi - asin(vect_prod / (abs_speed * abs_aim));
			}
			else
				if ((vect_prod < 0) && (scalar_prod > 0))
				{
					alpha = asin(vect_prod / (abs_speed * abs_aim));
				}
	 
	//������������ �� ��������� ����
	if (abs_aim < eps)
	{
		k++;
	}
	
	return alpha;
}