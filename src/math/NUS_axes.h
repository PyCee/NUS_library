#ifndef NUS_AXES_H
#define NUS_AXES_H

#include "NUS_vector.h"
#include "../NUS_bool.h"

struct NUS_quaternion;

typedef struct NUS_axes{
  NUS_vector forward, upward, right;
} NUS_axes;

NUS_axes nus_axes_build(NUS_vector, NUS_vector, NUS_vector);
NUS_axes nus_axes_build_default(void);
NUS_axes nus_axes_lerp(NUS_axes, NUS_axes, double);
NUS_axes nus_axes_inverse(NUS_axes);
NUS_axes nus_axes_normalize(NUS_axes);
void nus_axes_print(NUS_axes);

NUS_axes nus_axes_local_pitch(NUS_axes, double);
NUS_axes nus_axes_local_yaw(NUS_axes, double);
NUS_axes nus_axes_local_roll(NUS_axes, double);
NUS_axes nus_axes_global_pitch(NUS_axes, double);
NUS_axes nus_axes_global_yaw(NUS_axes, double);
NUS_axes nus_axes_global_roll(NUS_axes, double);
NUS_axes nus_axes_global_rotation(NUS_axes, struct NUS_quaternion);

NUS_bool nus_axes_cmp(NUS_axes, NUS_axes, float);

#endif /* NUS_AXES_H */
