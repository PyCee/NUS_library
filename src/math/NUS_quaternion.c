#include "NUS_quaternion.h"
#include "NUS_vector.h"
#include <math.h>
#include <stdio.h>

NUS_quaternion nus_quaternion_build(double w, double x, double y, double z)
{
  return (NUS_quaternion){w, x, y, z};
}
NUS_quaternion nus_quaternion_pure(NUS_vector NUS_vector_)
{
  return nus_quaternion_build(0.0, NUS_vector_.x, NUS_vector_.y, NUS_vector_.z);
}
NUS_quaternion nus_quaternion_unit(NUS_vector NUS_vector_, double radians)
{
  
  double half_radians = radians / 2.0,
    sin_half_radians = sin(half_radians);
  return nus_quaternion_build(cos(half_radians), sin_half_radians * NUS_vector_.x,
			      sin_half_radians * NUS_vector_.y,
			      sin_half_radians * NUS_vector_.z);
}
void nus_quaternion_print(NUS_quaternion NUS_quaternion_)
{
  printf("NUS_quaternion:\n{%f, %f, %f, %f}\n", NUS_quaternion_.w, NUS_quaternion_.x,
	 NUS_quaternion_.y, NUS_quaternion_.z);
}
NUS_quaternion nus_quaternion_conjugate(NUS_quaternion NUS_quaternion_)
{
  return nus_quaternion_build(NUS_quaternion_.w, NUS_quaternion_.x * -1.0,
			      NUS_quaternion_.y * -1.0, NUS_quaternion_.z * -1.0);
}
NUS_quaternion nus_quaternion_h_product
(NUS_quaternion NUS_quaternion_1,  NUS_quaternion NUS_quaternion_2)
{
  return nus_quaternion_build(
			      /* h product w component */
			      NUS_quaternion_1.w * NUS_quaternion_2.w -
			      NUS_quaternion_1.x * NUS_quaternion_2.x -
			      NUS_quaternion_1.y * NUS_quaternion_2.y -
			      NUS_quaternion_1.z * NUS_quaternion_2.z,
			      /* h product x component */
			      NUS_quaternion_1.w * NUS_quaternion_2.x +
			      NUS_quaternion_1.x * NUS_quaternion_2.w +
			      NUS_quaternion_1.y * NUS_quaternion_2.z -
			      NUS_quaternion_1.z * NUS_quaternion_2.y,
			      /* h product y component */
			      NUS_quaternion_1.w * NUS_quaternion_2.y -
			      NUS_quaternion_1.x * NUS_quaternion_2.z +
			      NUS_quaternion_1.y * NUS_quaternion_2.w +
			      NUS_quaternion_1.z * NUS_quaternion_2.x,
			      /* h product z component */
			      NUS_quaternion_1.w * NUS_quaternion_2.z +
			      NUS_quaternion_1.x * NUS_quaternion_2.y -
			      NUS_quaternion_1.y * NUS_quaternion_2.x +
			      NUS_quaternion_1.z * NUS_quaternion_2.w);
}

NUS_vector nus_quaternion_apply_rotation
(NUS_quaternion NUS_quaternion_, NUS_vector NUS_vector_)
{
  NUS_quaternion rotate = nus_quaternion_pure(NUS_vector_),
    conjugate = nus_quaternion_conjugate(NUS_quaternion_),
    result = nus_quaternion_h_product(nus_quaternion_h_product(NUS_quaternion_,
							       rotate),
				      conjugate);
  return nus_vector_build(result.x, result.y, result.z);
}
NUS_quaternion nus_quaternion_lerp
(NUS_quaternion NUS_quaternion_1, NUS_quaternion NUS_quaternion_2, double t)
{
  t = (t > 1) ? 1 : t;
  t = (t < 0) ? 0 : t;
  double b = 1.0 - t;
  return nus_quaternion_normalize(nus_quaternion_build(NUS_quaternion_1.w * b +
					       NUS_quaternion_2.w * t,
					       NUS_quaternion_1.x * b +
					       NUS_quaternion_2.x * t,
					       NUS_quaternion_1.y * b +
					       NUS_quaternion_2.y * t,
					       NUS_quaternion_1.z * b +
					       NUS_quaternion_2.z* t));
}
NUS_quaternion nus_quaternion_slerp
(NUS_quaternion NUS_quaternion_1, NUS_quaternion NUS_quaternion_2, double t)
{
  double wp, wq, angle, sin_angle;
  t = (t > 1) ? 1 : t;
  t = (t < 0) ? 0 : t;
  
  angle = acos(NUS_quaternion_1.w * NUS_quaternion_2.w +
	       NUS_quaternion_1.x * NUS_quaternion_2.x +
	       NUS_quaternion_1.y * NUS_quaternion_2.y +
	       NUS_quaternion_1.z * NUS_quaternion_2.z);
  sin_angle = sin(angle);
  wp = sin((1 - t) * angle) / sin_angle;
  wq = sin(t * angle) / sin_angle;
  return nus_quaternion_normalize(nus_quaternion_build(NUS_quaternion_1.w * wp +
						       NUS_quaternion_2.w * wq,
						       NUS_quaternion_1.x * wp +
						       NUS_quaternion_2.x * wq,
						       NUS_quaternion_1.y * wp +
						       NUS_quaternion_2.y * wq,
						       NUS_quaternion_1.z * wp +
						       NUS_quaternion_2.z* wq));
}
NUS_quaternion nus_quaternion_normalize(NUS_quaternion NUS_quaternion_)
{
  double mag_sq = NUS_quaternion_.w * NUS_quaternion_.w +
    NUS_quaternion_.x * NUS_quaternion_.x + NUS_quaternion_.y * NUS_quaternion_.y +
    NUS_quaternion_.z * NUS_quaternion_.z;
  if(0.00000001 >= mag_sq){
    return NUS_quaternion_;
  }
  double inv_mag = 1.0 / sqrt(mag_sq);
  return nus_quaternion_build(NUS_quaternion_.w * inv_mag, NUS_quaternion_.x * inv_mag,
			      NUS_quaternion_.y * inv_mag, NUS_quaternion_.z * inv_mag);
}
