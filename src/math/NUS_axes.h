#ifndef NUS_AXES_H
#define NUS_AXES_H

#include "NUS_vector.h"

struct NUS_quaternion;

typedef struct NUS_axes{
  NUS_vector forward, upward, left;
} NUS_axes;

NUS_axes nus_axes_build(NUS_vector, NUS_vector, NUS_vector);
NUS_axes nus_axes_interpolate(NUS_axes, NUS_axes, double);
NUS_axes nus_axes_invert(NUS_axes);

NUS_axes nus_axes_local_pitch(NUS_axes, double);
NUS_axes nus_axes_local_yaw(NUS_axes, double);
NUS_axes nus_axes_local_roll(NUS_axes, double);
NUS_axes nus_axes_global_pitch(NUS_axes, double);
NUS_axes nus_axes_global_yaw(NUS_axes, double);
NUS_axes nus_axes_global_roll(NUS_axes, double);
NUS_axes nus_axes_global_rotation(NUS_axes, struct NUS_quaternion);

#endif /* NUS_AXES_H */
