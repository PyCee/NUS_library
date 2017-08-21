#ifndef NUS_PHYSICS_STATE_H
#define NUS_PHYSICS_STATE_H

#include "../math/NUS_vector.h"
#include "../math/NUS_axes.h"

#define NUS_PHYSICS_STEP_S 0.016

struct NUS_movement;
struct NUS_orientation;

typedef struct NUS_physics_state{
  NUS_vector position;
  NUS_axes local_orientation;
  //NUS_axes head_orientation; // add after animation-blending is created
} NUS_physics_state;

NUS_physics_state nus_physics_state_build(void);
void nus_physics_state_free(NUS_physics_state *);
NUS_physics_state nus_physics_state_lerp
(NUS_physics_state, NUS_physics_state, double);
NUS_physics_state nus_physics_state_step
(NUS_physics_state, struct NUS_movement *, struct NUS_orientation *);
void nus_physics_state_print(NUS_physics_state);

#endif /* NUS_PHYSICS_STATE_H */
