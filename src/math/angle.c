#include "angle.h"

double radians(double deg)
{
	return deg * RAD_PER_DEG;
}
double degrees(double rad)
{
	return rad * DEG_PER_RAD;
}
