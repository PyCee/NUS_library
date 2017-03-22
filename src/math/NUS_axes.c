#include "NUS_axes.h"
#include "NUS_quaternion.h"
#include <stdio.h>

NUS_axes nus_axes_build(NUS_vector forward, NUS_vector upward, NUS_vector left)
{
  return (NUS_axes){forward, upward, left};
}
NUS_axes nus_axes_interpolate(NUS_axes NUS_axes_1, NUS_axes NUS_axes_2, double t)
{
  return nus_axes_build(nus_vector_interpolate(NUS_axes_1.forward,
					       NUS_axes_2.forward, t),
		        nus_vector_interpolate(NUS_axes_1.upward,
					       NUS_axes_2.upward, t),
			nus_vector_interpolate(NUS_axes_1.left,
					       NUS_axes_2.left, t));
}
NUS_axes nus_axes_invert(NUS_axes NUS_axes_)
{
  return nus_axes_build(nus_vector_scale(NUS_axes_.forward, -1.0),
			nus_vector_scale(NUS_axes_.upward, -1.0),
			nus_vector_scale(NUS_axes_.left, -1.0));
}
void nus_axes_print(NUS_axes NUS_axes_)
{
  printf("forward: ");
  nus_vector_print(NUS_axes_.forward);
  printf("upward: ");
  nus_vector_print(NUS_axes_.upward);
  printf("left: ");
  nus_vector_print(NUS_axes_.left);
}
NUS_axes nus_axes_local_pitch(NUS_axes NUS_axes_, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(NUS_axes_.left, radians);
  return nus_axes_build(nus_quaternion_apply_rotation(rotation, NUS_axes_.forward),
			nus_quaternion_apply_rotation(rotation, NUS_axes_.upward),
			NUS_axes_.left);
}
NUS_axes nus_axes_local_yaw(NUS_axes NUS_axes_, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(NUS_axes_.upward, radians);
  return nus_axes_build(NUS_axes_.forward,
			nus_quaternion_apply_rotation(rotation, NUS_axes_.upward),
			nus_quaternion_apply_rotation(rotation, NUS_axes_.left));
}
NUS_axes nus_axes_local_roll(NUS_axes NUS_axes_, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(NUS_axes_.forward, radians);
  return nus_axes_build(nus_quaternion_apply_rotation(rotation, NUS_axes_.forward),
			NUS_axes_.upward,
			nus_quaternion_apply_rotation(rotation, NUS_axes_.left));
}
NUS_axes nus_axes_global_pitch(NUS_axes NUS_axes_, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(NUS_axes_.left, radians);
  return nus_axes_global_rotation(NUS_axes_, rotation);
}
NUS_axes nus_axes_global_yaw(NUS_axes NUS_axes_, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(NUS_axes_.upward, radians);
  return nus_axes_global_rotation(NUS_axes_, rotation);
}
NUS_axes nus_axes_global_roll(NUS_axes NUS_axes_, double radians)
{
  NUS_quaternion rotation = nus_quaternion_unit(NUS_axes_.forward, radians);
  return nus_axes_global_rotation(NUS_axes_, rotation);
}
NUS_axes nus_axes_global_rotation(NUS_axes NUS_axes_, NUS_quaternion NUS_quaternion_)
{
  return nus_axes_build(nus_quaternion_apply_rotation(NUS_quaternion_,
						      NUS_axes_.forward),
			nus_quaternion_apply_rotation(NUS_quaternion_,
						      NUS_axes_.upward),
			nus_quaternion_apply_rotation(NUS_quaternion_,
						      NUS_axes_.left));
}
