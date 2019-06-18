#include "Navigation.h"

void Turn_control(float OMEGA, float Velocity, float Current_OmegaZ, float *Omega_LR_required, bool straight)
{
	static float const Wheel_radius = 0.03375; //from prev function
	float omega_const_1 = 0, omega_const_2 = 0;
	float Radius = Velocity/OMEGA, omega_additional = 0;
	
	omega_const_1 = 2*Velocity/Wheel_radius;
	omega_const_2 = 2*Velocity/Wheel_radius;
	
	omega_additional = Calc_Omega_additional_using_gyro(Current_OmegaZ, OMEGA, 5.0, 1.0); //10, 0.1
	

	Omega_LR_required[0] = 9.55*(omega_const_1 - omega_additional/2); 
	Omega_LR_required[1] = 9.55*(omega_const_2 + omega_additional/2); 
}
double getOmegaAim(double lambda_aim, double phi_aim, double lambda_tek, double phi_tek, double hdg, double abs_speed)
{
	//latitude - phi, longitude - lambda
	
	double x_aim = 0, y_aim = 0; //координаты цели в новой ортодромической СК
	double speed_x = 0, speed_y = 0; //вектор скорости в ортодромической СК
	double abs_aim = 0; // модуль вектора направления на цель
	double vect_prod = 0; // переменная для хранения вертикальной компоненты векторного произведения
	double scalar_prod = 0; //скалярное произведение векторов
	double alpha = 0; // угол на цель

	const long Earth_Radius = 6371000; //радиус Земли
	const float pi = 3.14159265;


	x_aim = ((lambda_aim - lambda_tek)*pi / 180)*Earth_Radius;
	y_aim = ((phi_aim - phi_tek)*pi / 180)*Earth_Radius; //перевод в ортодромические координаты, инициализация СК в начальной точке
	
	abs_aim = sqrt(pow(x_aim, 2) + pow(y_aim, 2)); //расчет модуля вектора направления на цель
	
	hdg = hdg * pi / 180; //перевод в радианы

	// координаты вектора текущей скорости в ортодромической СК
	if (hdg <= (pi / 2))
	{
		speed_x = abs_speed * sin(hdg);
		speed_y = abs_speed * cos(hdg);
	}
	else
		if ((hdg > (pi / 2)) && (hdg <= pi))
		{
			speed_x = abs_speed * sin(pi - hdg);
			speed_y = -abs_speed * cos(pi - hdg);
		}
		else
			if ((hdg > pi) && (hdg <= (3 * pi / 2)))
			{
				speed_x = -abs_speed * sin(hdg - pi);
				speed_y = -abs_speed * cos(hdg - pi);
			}
			else
				if ((hdg > (3 * pi / 2)) && (hdg <= (2 * pi)))
				{
					speed_x = -abs_speed * sin(2 * pi - hdg);
					speed_y = abs_speed * cos(2 * pi - hdg);
				}

	vect_prod = speed_x * y_aim - speed_y * x_aim; //вертикальная компонента векторного произведения вектора на цель и скорости
	scalar_prod = speed_x * x_aim + speed_y * y_aim; // скалярное произведение векторов в плоскости местного горизонта

	//расчет угла на цель через модуль векторного произведения, для однозначного определения координатной четверти исп sin и cos (векторное и скалярное пр)
	if ((vect_prod >= 0) && (scalar_prod >= 0))
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
	
	return alpha;
}