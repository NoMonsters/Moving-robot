#include "pch.h"
#include "math.h"

double getOmegaAim(double lambda_aim, double phi_aim, double lambda_tek, double phi_tek, double hdg, double abs_speed)
{
	double x_aim, y_aim; //координаты цели в новой ортодромической СК
	double speed_x, speed_y; //вектор скорости в ортодромической СК
	double abs_aim; // модуль вектора направления на цель
	double vect_prod; // переменная для хранения вертикальной компоненты векторного произведения
	double alpha; // угол на цель

	const long Earth_Radius = 6371000; //радиус Земли
	const float pi = 3.14159265;

	x_aim = ((lambda_aim - lambda_tek)*pi / 180)*Earth_Radius;
	y_aim = ((phi_aim - phi_tek)*pi / 180)*Earth_Radius; //перевод в ортодромические координаты, инициализация СК в начальной точке

	speed_x = abs_speed * sin(hdg);
	speed_y = abs_speed * cos(hdg); // координаты вектора текущей скорости в ортодромической СК

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

	alpha = alpha * 180 / pi; //перевод в градусы (для индикации и проверки)
	return alpha;
}