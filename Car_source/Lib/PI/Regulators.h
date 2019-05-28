#ifndef REGULATORS_H_					/* Define library H file if not defined */
#define REGULATORS_H_

const double Max_output_for_Reg = 255;
const double Min_output_for_Reg = 0;

double Apply_regulator_left(double, double, float, float);
double Apply_regulator_right(double, double, float, float);
double Calc_Omega_additional_using_gyro(double, double, float, float);

#endif	