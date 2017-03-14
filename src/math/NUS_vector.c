#include "NUS_vector.h"
#include "stdio.h"
#include <math.h>

NUS_vector nus_vector_build(double x, double y, double z)
{
  return (NUS_vector){x, y, z};
}
NUS_vector nus_vector_add(NUS_vector NUS_vector_0, NUS_vector NUS_vector_1)
{
  return nus_vector_build(NUS_vector_0.x + NUS_vector_1.x,
			  NUS_vector_0.y + NUS_vector_1.y,
			  NUS_vector_0.z + NUS_vector_1.z);
}
NUS_vector nus_vector_scale(NUS_vector NUS_vector_, double scale)
{
  return nus_vector_build(NUS_vector_.x * scale,
			  NUS_vector_.y * scale,
			  NUS_vector_.z * scale);
}
NUS_vector nus_vector_subtract(NUS_vector NUS_vector_0, NUS_vector NUS_vector_1)
{
  return nus_vector_add(NUS_vector_0, nus_vector_scale(NUS_vector_1, -1));
}
NUS_vector nus_vector_normalize(NUS_vector NUS_vector_)
{
  double mag_sq = nus_vector_dot(NUS_vector_, NUS_vector_);
  if(0.00000001 > mag_sq){
    return NUS_vector_;
  }
  return nus_vector_scale(NUS_vector_, 1.0 / sqrt(mag_sq));
}
double nus_vector_dot(NUS_vector NUS_vector_0, NUS_vector NUS_vector_1)
{
  return NUS_vector_0.x * NUS_vector_1.x +
    NUS_vector_0.y * NUS_vector_1.y +
    NUS_vector_0.z * NUS_vector_1.z;
}
NUS_vector nus_vector_cross(NUS_vector NUS_vector_0, NUS_vector NUS_vector_1)
{
  return nus_vector_build(NUS_vector_0.y * NUS_vector_1.z -
			  NUS_vector_0.z * NUS_vector_1.y,
			  NUS_vector_0.z * NUS_vector_1.x -
			  NUS_vector_0.x * NUS_vector_1.z,
			  NUS_vector_0.x * NUS_vector_1.y -
			  NUS_vector_0.y * NUS_vector_1.x);
}
NUS_vector nus_vector_interpolate
(NUS_vector NUS_vector_0, NUS_vector NUS_vector_1, double t)
{
  double b;
  t = (t < 1.0) ? t : 1.0;
  t = (t > 0.0) ? t : 0.0;
  b = 1.0 - t;
  return nus_vector_add(nus_vector_scale(NUS_vector_0, b),
			nus_vector_scale(NUS_vector_1, t));
}
char nus_vector_cmp(NUS_vector NUS_vector_0, NUS_vector NUS_vector_1, double range)
{
  NUS_vector sub = nus_vector_subtract(NUS_vector_0, NUS_vector_1);
  return nus_vector_dot(sub, sub) <=
    range * range;
}
void nus_vector_print(NUS_vector NUS_vector_)
{
  printf("{%f, %f, %f}\n", NUS_vector_.x, NUS_vector_.y, NUS_vector_.z);
}
