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
	double x_aim = 0, y_aim = 0; //координаты цели в новой ортодромической — 
	double speed_x = 0, speed_y = 0; //вектор скорости в ортодромической — 
	double abs_aim = 0; // модуль вектора направлени€ на цель
	double vect_prod = 0; // переменна€ дл€ хранени€ вертикальной компоненты векторного произведени€
	double alpha = 0; // угол на цель
	static int k = 0; // номер точки, к которой едем
	double eps = 1.0; // радиус окрестности точки (точность)

	const long Earth_Radius = 6371000; //радиус «емли
	const float pi = 3.14159265;


	x_aim = ((lambda_aim[k] - lambda_tek)*pi / 180)*Earth_Radius;
	y_aim = ((phi_aim[k] - phi_tek)*pi / 180)*Earth_Radius; //перевод в ортодромические координаты, инициализаци€ —  в начальной точке
	
	abs_aim = sqrt(pow(x_aim, 2) + pow(y_aim, 2)); //расчет модул€ вектора направлени€ на цель
	

	hdg = hdg * pi / 180; //перевод в радианы

	// координаты вектора текущей скорости в ортодромической — 
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

	vect_prod = speed_x * y_aim - speed_y * x_aim; //вертикальна€ компонента векторного произведени€ вектора на цель и скорости

	alpha = asin(vect_prod / (abs_speed*abs_aim)); //расчет угла на цель через модуль векторного произведени€

	 //тк арксинус считает от -pi/2 до pi/2 при переходе цели ниже оси х нужно пересчитать
	if ((phi_aim[k] < phi_tek) && (lambda_aim[k] > lambda_tek))
	{
		alpha = -alpha - pi;
	}

	if ((phi_aim[k] < phi_tek) && (lambda_aim[k] < lambda_tek))
	{
		alpha = pi - alpha;
	}
	 
	//переключение на следующую цель
	if (abs_aim < eps)
	{
		k++;
	}
	
	return alpha;
}