#ifndef NAVIGATION_H_					/* Define library H file if not defined */
#define NAVIGATION_H_

#include "math.h"
#include "../PI/Regulators.h"	

void Turn_control(float, float, float, float *, bool);
double getOmegaAim(double, double, double, double, double, double);

#endif