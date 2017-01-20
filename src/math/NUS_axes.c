#include "NUS_axes.h"

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

NUS_axes nus_axes_local_pitch(NUS_axes, double);
NUS_axes nus_axes_local_yaw(NUS_axes, double);
NUS_axes nus_axes_local_roll(NUS_axes, double);
NUS_axes nus_axes_global_pitch(NUS_axes, double);
NUS_axes nus_axes_global_yaw(NUS_axes, double);
NUS_axes nus_axes_global_roll(NUS_axes, double);
NUS_axes nus_axes_global_rotation(NUS_axes, struct NUS_quaternion);
