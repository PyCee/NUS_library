#include "cartesian_system.h"
#include "angle.h"
#include "vector.h"
#include "quaternion.h"

#include <stdlib.h>

cartesian_system gen_cartesian_system(void)
{
  static cartesian_system _cartesian_system = DEFAULT_CARTESIAN_SYSTEM;
  return _cartesian_system;
}
cartesian_system build_cartesian_system(vector forward, vector upward, vector left)
{
  cartesian_system sys = gen_cartesian_system();
  sys.forward = forward;
  sys.upward = upward;
  sys.left = left;
  return sys;
}
cartesian_system interpolate_cartesian_system
(cartesian_system cartesian_system_1, cartesian_system cartesian_system_2, float t)
{
  t = (t < 1.0) ? t : 1.0;
  t = (t > 0.0) ? t : 0.0;
  return build_cartesian_system(interpolate_vector(cartesian_system_1.forward,
						   cartesian_system_2.forward, t),
				interpolate_vector(cartesian_system_1.upward,
						   cartesian_system_2.upward, t),
				interpolate_vector(cartesian_system_1.left,
						   cartesian_system_2.left, t));
}
void invert_cartesian_system(cartesian_system *sys)
{
  sys->forward = scale_vector(sys->forward, -1.0);
  sys->upward = scale_vector(sys->upward, -1.0);
  sys->left = scale_vector(sys->left, -1.0);
}
void cartesian_local_pitch(cartesian_system *sys, float rad)
{
  quaternion rot = build_unit_quaternion(sys->left, rad);
  sys->forward = apply_rotation_quaternion(sys->forward, rot);
  sys->upward = apply_rotation_quaternion(sys->upward, rot);
}
void cartesian_local_yaw(cartesian_system *sys, float rad)
{
  quaternion rot = build_unit_quaternion(sys->left, rad);
  sys->forward = apply_rotation_quaternion(sys->forward, rot);
  sys->left = apply_rotation_quaternion(sys->left, rot);
}
void cartesian_local_roll(cartesian_system *sys, float rad)
{
  quaternion rot = build_unit_quaternion(sys->forward, rad);
  sys->upward = apply_rotation_quaternion(sys->upward, rot);
  sys->left = apply_rotation_quaternion(sys->left, rot);
}
void cartesian_global_pitch(cartesian_system *sys, float rad)
{
  quaternion rot = build_unit_quaternion(build_vector(1.0, 0.0, 0.0), rad);
  cartesian_apply_global_rotation(sys, rot);
}
void cartesian_global_yaw(cartesian_system *sys, float rad)
{
  quaternion rot = build_unit_quaternion(build_vector(0.0, 1.0, 0.0), rad);
  cartesian_apply_global_rotation(sys, rot);
}
void cartesian_global_roll(cartesian_system *sys, float rad)
{
  quaternion rot = build_unit_quaternion(build_vector(0.0, 0.0, 1.0), rad);
  cartesian_apply_global_rotation(sys, rot);
}
void cartesian_apply_global_rotation(cartesian_system *sys, quaternion rot)
{
  sys->forward = apply_rotation_quaternion(sys->forward, rot);
  sys->upward = apply_rotation_quaternion(sys->upward, rot);
  sys->left = apply_rotation_quaternion(sys->left, rot);
}
