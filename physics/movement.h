#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include <vector.h>
#include "kinematic_property.h"

#define GRAVITY_FORCE 9.8
#define DEFAULT_MOVEMENT {DEFAULT_VECTOR, DEFAULT_VECTOR, DEFAULT_KINEMATIC_PROPERTY, \
			  DEFAULT_KINEMATIC_PROPERTY, 1.0, -1, {0}}

/* alignment : 36 bits */
typedef struct movement{
  /* Class that handles displacement between physics_state steps */
  vector gravity, displacement;
  kinematic_property momentum, force;
  float mass;
  short gravity_id;
  char pad[2];
} movement;

movement gen_movement(void);
void free_movement(movement *);
struct physics_state update_physics_state_movement(struct physics_state, movement *);
void set_movement_gravity(movement *, vector, float);
void print_movement(movement);

#endif /* _MOVEMENT_H_ */
