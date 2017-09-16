#include "NUS_axes.h"
#include "NUS_quaternion.h"
#include <stdio.h>

NUS_axes nus_axes_build(NUS_vector forward, NUS_vector upward, NUS_vector right)
{
  return (NUS_axes){forward, upward, right};
}
NUS_axes nus_axes_build_default(void)
{
  return nus_axes_build(nus_vector_build(0.0, 0.0, -1.0),
			nus_vector_build(0.0, -1.0, 0.0),
			nus_vector_build(1.0, 0.0, 0.0));
}
NUS_axes nus_axes_lerp(NUS_axes axes_0, NUS_axes axes_1, double t)
{
  return nus_axes_build(nus_vector_lerp(axes_0.forward, axes_1.forward, t),
		        nus_vector_lerp(axes_0.upward, axes_1.upward, t),
			nus_vector_lerp(axes_0.right, axes_1.right, t));
}
NUS_axes nus_axes_inverse(NUS_axes axes)
{
  return nus_axes_build(nus_vector_scale(axes.forward, -1.0),
			nus_vector_scale(axes.upward, -1.0),
			nus_vector_scale(axes.right, -1.0));
}
void nus_axes_print(NUS_axes axes)
{
  printf("forward: ");
  nus_vector_print(axes.forward);
  printf("upward: ");
  nus_vector_print(axes.upward);
  printf("right: ");
  nus_vector_print(axes.right);
}
NUS_axes nus_axes_local_pitch(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.right, radians);
  return nus_axes_build(nus_quaternion_apply_rotation(rotation, axes.forward),
			nus_quaternion_apply_rotation(rotation, axes.upward),
			axes.right);
}
NUS_axes nus_axes_local_yaw(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.upward, radians);
  return nus_axes_build(axes.forward,
			nus_quaternion_apply_rotation(rotation, axes.upward),
			nus_quaternion_apply_rotation(rotation, axes.right));
}
NUS_axes nus_axes_local_roll(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.forward, radians);
  return nus_axes_build(nus_quaternion_apply_rotation(rotation, axes.forward),
			axes.upward,
			nus_quaternion_apply_rotation(rotation, axes.right));
}
NUS_axes nus_axes_global_pitch(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.right, radians);
  return nus_axes_global_rotation(axes, rotation);
}
NUS_axes nus_axes_global_yaw(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.upward, radians);
  return nus_axes_global_rotation(axes, rotation);
}
NUS_axes nus_axes_global_roll(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.forward, radians);
  return nus_axes_global_rotation(axes, rotation);
}
NUS_axes nus_axes_global_rotation(NUS_axes axes, NUS_quaternion quaternion)
{
  return nus_axes_build(nus_quaternion_apply_rotation(quaternion,
						      axes.forward),
			nus_quaternion_apply_rotation(quaternion,
						      axes.upward),
			nus_quaternion_apply_rotation(quaternion,
						      axes.right));
}

NUS_bool nus_axes_cmp(NUS_axes axes_0, NUS_axes axes_1, float d)
{
  if(nus_vector_cmp(axes_0.forward, axes_1.forward, d) &&
     nus_vector_cmp(axes_0.upward, axes_1.upward, d) &&
     nus_vector_cmp(axes_0.right, axes_1.right, d)){
    return NUS_TRUE;
  }
  return NUS_FALSE;
}
