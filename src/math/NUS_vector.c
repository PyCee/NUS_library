#include "NUS_vector.h"
#include "stdio.h"
#include <math.h>

NUS_vector nus_vector_build(double x, double y, double z)
{
  return (NUS_vector){x, y, z};
}
NUS_vector nus_vector_add(NUS_vector vector_0, NUS_vector vector_1)
{
  return nus_vector_build(vector_0.x + vector_1.x,
			  vector_0.y + vector_1.y,
			  vector_0.z + vector_1.z);
}
NUS_vector nus_vector_scale(NUS_vector vector, double scale)
{
  return nus_vector_build(vector.x * scale,
			  vector.y * scale,
			  vector.z * scale);
}
NUS_vector nus_vector_subtract(NUS_vector vector_0, NUS_vector vector_1)
{
  return nus_vector_add(vector_0, nus_vector_scale(vector_1, -1));
}
NUS_vector nus_vector_normalize(NUS_vector vector)
{
  double mag_sq = nus_vector_dot(vector, vector);
  if(0.00000001 > mag_sq){
    return vector;
  }
  return nus_vector_scale(vector, 1.0 / sqrt(mag_sq));
}
double nus_vector_dot(NUS_vector vector_0, NUS_vector vector_1)
{
  return vector_0.x * vector_1.x +
    vector_0.y * vector_1.y +
    vector_0.z * vector_1.z;
}
NUS_vector nus_vector_cross(NUS_vector vector_0, NUS_vector vector_1)
{
  return nus_vector_build(vector_0.y * vector_1.z -
			  vector_0.z * vector_1.y,
			  vector_0.z * vector_1.x -
			  vector_0.x * vector_1.z,
			  vector_0.x * vector_1.y -
			  vector_0.y * vector_1.x);
}
NUS_vector nus_vector_interpolate
(NUS_vector vector_0, NUS_vector vector_1, double t)
{
  double b;
  t = (t < 1.0) ? t : 1.0;
  t = (t > 0.0) ? t : 0.0;
  b = 1.0 - t;
  return nus_vector_add(nus_vector_scale(vector_0, b),
			nus_vector_scale(vector_1, t));
}
char nus_vector_cmp(NUS_vector vector_0, NUS_vector vector_1, double range)
{
  NUS_vector sub = nus_vector_subtract(vector_0, vector_1);
  return nus_vector_dot(sub, sub) <= range * range;
}
void nus_vector_print(NUS_vector vector)
{
  printf("{%f, %f, %f}\n", vector.x, vector.y, vector.z);
}
