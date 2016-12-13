#include "pythag.h"
#include <math.h>

double pythag(float x, float y, float z)
{
	return sqrt(x * x + y * y + z * z);
	// return q_rsqrt(x * x + y * y + z * z);
}
