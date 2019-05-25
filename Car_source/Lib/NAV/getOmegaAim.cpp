#include "math.h"

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