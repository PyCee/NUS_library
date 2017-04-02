#include "NUS_axes.h"
#include "NUS_quaternion.h"
#include <stdio.h>

NUS_axes nus_axes_build(NUS_vector forward, NUS_vector upward, NUS_vector left)
{
  return (NUS_axes){forward, upward, left};
}
NUS_axes nus_axes_interpolate(NUS_axes axes_1, NUS_axes axes_2, double t)
{
  return nus_axes_build(nus_vector_interpolate(axes_1.forward,
					       axes_2.forward, t),
		        nus_vector_interpolate(axes_1.upward,
					       axes_2.upward, t),
			nus_vector_interpolate(axes_1.left,
					       axes_2.left, t));
}
NUS_axes nus_axes_invert(NUS_axes axes)
{
  return nus_axes_build(nus_vector_scale(axes.forward, -1.0),
			nus_vector_scale(axes.upward, -1.0),
			nus_vector_scale(axes.left, -1.0));
}
void nus_axes_print(NUS_axes axes)
{
  printf("forward: ");
  nus_vector_print(axes.forward);
  printf("upward: ");
  nus_vector_print(axes.upward);
  printf("left: ");
  nus_vector_print(axes.left);
}
NUS_axes nus_axes_local_pitch(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.left, radians);
  return nus_axes_build(nus_quaternion_apply_rotation(rotation, axes.forward),
			nus_quaternion_apply_rotation(rotation, axes.upward),
			axes.left);
}
NUS_axes nus_axes_local_yaw(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.upward, radians);
  return nus_axes_build(axes.forward,
			nus_quaternion_apply_rotation(rotation, axes.upward),
			nus_quaternion_apply_rotation(rotation, axes.left));
}
NUS_axes nus_axes_local_roll(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.forward, radians);
  return nus_axes_build(nus_quaternion_apply_rotation(rotation, axes.forward),
			axes.upward,
			nus_quaternion_apply_rotation(rotation, axes.left));
}
NUS_axes nus_axes_global_pitch(NUS_axes axes, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(axes.left, radians);
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
						      axes.left));
}
