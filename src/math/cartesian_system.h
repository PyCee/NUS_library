#ifndef _CARTESIAN_SYSTEM_H_
#define _CARTESIAN_SYSTEM_H_

#include "vector.h"

#define DEFAULT_CARTESIAN_SYSTEM {{0.0, 0.0, -1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0}}

struct quaternion;

/* alignment : 36 bits */
typedef struct cartesian_system{
  vector forward, upward, left;
} cartesian_system;

cartesian_system gen_cartesian_system(void);
cartesian_system build_cartesian_system(vector, vector, vector);
cartesian_system interpolate_cartesian_system(cartesian_system, cartesian_system, float);

void copy_cartesian_system(cartesian_system *, cartesian_system *);
void invert_cartesian_system(cartesian_system *);

void cartesian_local_pitch(cartesian_system *, float);
void cartesian_local_yaw(cartesian_system *, float);
void cartesian_local_roll(cartesian_system *, float);
void cartesian_global_pitch(cartesian_system *, float);
void cartesian_global_yaw(cartesian_system *, float);
void cartesian_global_roll(cartesian_system *, float);
void cartesian_apply_global_rotation(cartesian_system *, struct quaternion);

#endif /* _CARTESIAN_SYSTEM_H_ */
