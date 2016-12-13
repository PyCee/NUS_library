#ifndef _PHYSICS_STATE_H_
#define _PHYSICS_STATE_H_

#include <matrix.h>
#include <vector.h>
#include <cartesian_system.h>

#define PHYSICS_STEP_S 0.016
#define DEFAULT_PHYSICS_STATE {DEFAULT_VECTOR, DEFAULT_CARTESIAN_SYSTEM, \
			       DEFAULT_CARTESIAN_SYSTEM}

struct movement;
struct orientation;

/* alignment : 36 bits */
typedef struct physics_state{
  vector position;
  cartesian_system local_orientation, head_orientation;
} physics_state;

physics_state gen_physics_state(void);
void free_physics_state(physics_state *);
physics_state interpolate_physics_states(physics_state, physics_state, float);
physics_state step_physics_state(physics_state, struct movement *, struct orientation *);
void print_physics_state(physics_state);

#endif /* _PHYSICS_STATE_H_ */
