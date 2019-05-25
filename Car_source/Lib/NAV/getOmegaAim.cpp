#include "math.h"

double getOmegaAim(double lambda_aim, double phi_aim, double lambda_tek, double phi_tek, double hdg, double abs_speed)
{
	double x_aim = 0, y_aim = 0; //координаты цели в новой ортодромической СК
	double speed_x = 0, speed_y = 0; //вектор скорости в ортодромической СК
	double abs_aim = 0; // модуль вектора направления на цель
	double vect_prod = 0; // переменная для хранения вертикальной компоненты векторного произведения
	double alpha = 0; // угол на цель

	const long Earth_Radius = 6371000; //радиус Земли
	const float pi = 3.14159265;

	x_aim = ((lambda_aim - lambda_tek)*pi / 180)*Earth_Radius;
	y_aim = ((phi_aim - phi_tek)*pi / 180)*Earth_Radius; //перевод в ортодромические координаты, инициализация СК в начальной точке

	hdg = hdg * pi / 180; //перевод в радианы

	// координаты вектора текущей скорости в ортодромической СК
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

	abs_aim = sqrt(pow(x_aim, 2) + pow(y_aim, 2)); //расчет модуля вектора направления на цель

	vect_prod = speed_x * y_aim - speed_y * x_aim; //вертикальная компонента векторного произведения вектора на цель и скорости

	alpha = asin(vect_prod / (abs_speed*abs_aim)); //расчет угла на цель через модуль векторного произведения

	 //тк арксинус считает от -pi/2 до pi/2 при переходе цели ниже оси х нужно пересчитать
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