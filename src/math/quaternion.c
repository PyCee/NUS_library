#include "quaternion.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "matrix.h"
#include "angle.h"

static quaternion normalize_quaternion(quaternion);

quaternion gen_quaternion(void)
{
	quaternion qua;
	qua.w = 0;
	qua.x = 0;
	qua.y = 0;
	qua.z = 0;
	return qua;
}
quaternion build_quaternion(float W, float X, float Y, float Z)
{
	quaternion qua;
	qua.w = W;
	qua.x = X;
	qua.y = Y;
	qua.z = Z;
	return qua;
}
void print_quaternion(quaternion qua)
{
	printf("[ %f, %f, %f, %f]\n", qua.w, qua.x, qua.y, qua.z);
}
quaternion build_pure_quaternion(vector vec)
{
	return build_quaternion(0.0, vec.x, vec.y, vec.z);
}
quaternion build_unit_quaternion(vector vec, float rad)
{
	float half_rad = rad / 2.0;
	float sin_half_rad = sin(half_rad);
	return build_quaternion(cos(half_rad), sin_half_rad * vec.x, sin_half_rad * vec.y, sin_half_rad * vec.z);
}
quaternion conjugate_quaternion(quaternion qua_1)
{
	quaternion qua;
	qua.w = qua_1.w;
	qua.x = -1 * qua_1.x;
	qua.y = -1 * qua_1.y;
	qua.z = -1 * qua_1.z;
	return qua;
}
// returns Hamilton Product of Quaternions
quaternion h_product_quaternion(quaternion qua_1, quaternion qua_2)
{
	quaternion qua;
	qua.w = qua_1.w * qua_2.w - qua_1.x * qua_2.x - qua_1.y * qua_2.y - qua_1.z * qua_2.z;
	qua.x = qua_1.w * qua_2.x + qua_1.x * qua_2.w + qua_1.y * qua_2.z - qua_1.z * qua_2.y;
	qua.y = qua_1.w * qua_2.y - qua_1.x * qua_2.z + qua_1.y * qua_2.w + qua_1.z * qua_2.x;
	qua.z = qua_1.w * qua_2.z + qua_1.x * qua_2.y - qua_1.y * qua_2.x + qua_1.z * qua_2.w;
	return qua;
}
vector apply_rotation_quaternion(vector rotate, quaternion qua_around)
{
	quaternion qua_rotate = build_pure_quaternion(rotate);
	quaternion qua_conjugate = conjugate_quaternion(qua_around);
	quaternion qua = h_product_quaternion((h_product_quaternion(qua_around, qua_rotate)), qua_conjugate);
	return build_vector(qua.x, qua.y, qua.z);
}
quaternion lerp_quaternion(quaternion qua_1, quaternion qua_2, float t)
{
	t = (t > 1) ? 1 : t;
	t = (t < 0) ? 0 : t;
	float b = 1-t;
	return normalize_quaternion(build_quaternion(qua_1.w * b + qua_2.w * t,
												qua_1.x * b + qua_2.x * t,
												qua_1.y * b + qua_2.y * t,
												qua_1.z * b + qua_2.z* t));
}
quaternion slerp_quaternion(quaternion qua_1, quaternion qua_2, float t)
{
  float wp, wq, a, sa;
  t = (t > 1) ? 1 : t;
  t = (t < 0) ? 0 : t;
  a = acos(qua_1.w * qua_1.w + qua_1.x * qua_1.x + qua_1.y * qua_1.y + qua_1.z * qua_1.z);
  sa = sin(a);
  wp = sin((1 - t) * a) / sa;
  wq = sin(t * a) / sa;
  return normalize_quaternion(build_quaternion(qua_1.w * wp + qua_2.w * wq,
					       qua_1.x * wp + qua_2.x * wq,
					       qua_1.y * wp + qua_2.y * wq,
					       qua_1.z * wp + qua_2.z* wq));
}
static quaternion normalize_quaternion(quaternion qua_1)
{
	float change = sqrt(qua_1.w * qua_1.w + qua_1.x * qua_1.x + qua_1.y * qua_1.y + qua_1.z * qua_1.z);
	return build_quaternion(qua_1.w / change, qua_1.x / change, qua_1.y / change, qua_1.z / change);
}
