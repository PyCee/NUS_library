#include "NUS_quaternion.h"
#include "NUS_vector.h"
#include <math.h>
#include <stdio.h>

NUS_quaternion nus_quaternion_build(double w, double x, double y, double z)
{
  return (NUS_quaternion){w, x, y, z};
}
NUS_quaternion nus_quaternion_pure(NUS_vector vector)
{
  return nus_quaternion_build(0.0, vector.x, vector.y, vector.z);
}
NUS_quaternion nus_quaternion_unit(NUS_vector vector, double radians)
{
  
  double half_radians = radians / 2.0,
    sin_half_radians = sin(half_radians);
  return nus_quaternion_build(cos(half_radians), sin_half_radians * vector.x,
			      sin_half_radians * vector.y,
			      sin_half_radians * vector.z);
}
NUS_quaternion nus_quaternion_conjugate(NUS_quaternion quaternion)
{
  return nus_quaternion_build(quaternion.w, quaternion.x * -1.0,
			      quaternion.y * -1.0, quaternion.z * -1.0);
}
NUS_quaternion nus_quaternion_h_product
(NUS_quaternion quaternion_0,  NUS_quaternion quaternion_1)
{
  return nus_quaternion_build(
			      /* h product w component */
			      quaternion_0.w * quaternion_1.w -
			      quaternion_0.x * quaternion_1.x -
			      quaternion_0.y * quaternion_1.y -
			      quaternion_0.z * quaternion_1.z,
			      /* h product x component */
			      quaternion_0.w * quaternion_1.x +
			      quaternion_0.x * quaternion_1.w +
			      quaternion_0.y * quaternion_1.z -
			      quaternion_0.z * quaternion_1.y,
			      /* h product y component */
			      quaternion_0.w * quaternion_1.y -
			      quaternion_0.x * quaternion_1.z +
			      quaternion_0.y * quaternion_1.w +
			      quaternion_0.z * quaternion_1.x,
			      /* h product z component */
			      quaternion_0.w * quaternion_1.z +
			      quaternion_0.x * quaternion_1.y -
			      quaternion_0.y * quaternion_1.x +
			      quaternion_0.z * quaternion_1.w);
}

NUS_vector nus_quaternion_apply_rotation
(NUS_quaternion quaternion, NUS_vector vector)
{
  NUS_quaternion rotate = nus_quaternion_pure(vector),
    conjugate = nus_quaternion_conjugate(quaternion),
    result = nus_quaternion_h_product(nus_quaternion_h_product(quaternion,
							       rotate),
				      conjugate);
  return nus_vector_build(result.x, result.y, result.z);
}
NUS_quaternion nus_quaternion_lerp
(NUS_quaternion quaternion_0, NUS_quaternion quaternion_1, double t)
{
  t = (t > 1) ? 1 : t;
  t = (t < 0) ? 0 : t;
  double b = 1.0 - t;
  return nus_quaternion_normalize(nus_quaternion_build(quaternion_0.w * b +
					       quaternion_1.w * t,
					       quaternion_0.x * b +
					       quaternion_1.x * t,
					       quaternion_0.y * b +
					       quaternion_1.y * t,
					       quaternion_0.z * b +
					       quaternion_1.z* t));
}
NUS_quaternion nus_quaternion_slerp
(NUS_quaternion quaternion_0, NUS_quaternion quaternion_1, double t)
{
  double wp, wq, angle, sin_angle;
  t = (t > 1) ? 1 : t;
  t = (t < 0) ? 0 : t;
  
  angle = acos(quaternion_0.w * quaternion_1.w +
	       quaternion_0.x * quaternion_1.x +
	       quaternion_0.y * quaternion_1.y +
	       quaternion_0.z * quaternion_1.z);
  sin_angle = sin(angle);
  wp = sin((1 - t) * angle) / sin_angle;
  wq = sin(t * angle) / sin_angle;
  return nus_quaternion_normalize(nus_quaternion_build(quaternion_0.w * wp +
						       quaternion_1.w * wq,
						       quaternion_0.x * wp +
						       quaternion_1.x * wq,
						       quaternion_0.y * wp +
						       quaternion_1.y * wq,
						       quaternion_0.z * wp +
						       quaternion_1.z* wq));
}
NUS_quaternion nus_quaternion_normalize(NUS_quaternion quaternion)
{
  double mag_sq = quaternion.w * quaternion.w +
    quaternion.x * quaternion.x + quaternion.y * quaternion.y +
    quaternion.z * quaternion.z;
  if(0.00000001 >= mag_sq){
    return quaternion;
  }
  double inv_mag = 1.0 / sqrt(mag_sq);
  return nus_quaternion_build(quaternion.w * inv_mag, quaternion.x * inv_mag,
			      quaternion.y * inv_mag, quaternion.z * inv_mag);
}
void nus_quaternion_print(NUS_quaternion quaternion)
{
  printf("NUS_quaternion:\n{%f, %f, %f, %f}\n", quaternion.w, quaternion.x,
	 quaternion.y, quaternion.z);
}
