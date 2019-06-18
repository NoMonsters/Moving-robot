#include "Regulators.h"

double Apply_regulator_left(double current_value, double required_value, float Kp, float Ki)
{
	static double output = 0, error = 0;
	static double output_prev = 0, error_prev = 0;// выход и ошибка с предыдушей итерации

	if (output_prev >= Max_output_for_Reg || output_prev <= Min_output_for_Reg)
	Ki = 0;
	
	error = required_value - current_value;
	output = output_prev + Kp*(error-error_prev)+Ki*error;
	
	error_prev = error;
	output_prev = output;

	return output;
}

double Apply_regulator_right(double current_value, double required_value, float Kp, float Ki)
{
	static double output = 0, error = 0;
	static double output_prev = 0, error_prev = 0;// выход и ошибка с предыдушей итерации

	if (output_prev >= Max_output_for_Reg || output_prev <= Min_output_for_Reg)
	Ki = 0;

	error = required_value - current_value;
	output = output_prev + Kp*(error-error_prev)+Ki*error;
	
	error_prev = error;
	output_prev = output;

	return output;
}

double Calc_Omega_additional_using_gyro(double current_value, double required_value, float Kp, float Ki)
{
	static double output = 0, error = 0;
	static double output_prev = 0, error_prev = 0;// выход и ошибка с предыдушей итерации

	error = required_value - current_value;
	output = output_prev + Kp*(error-error_prev)+Ki*error;
	error_prev = error;
	output_prev = output;

	return output;
}