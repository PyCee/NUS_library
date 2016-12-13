#include "movement.h"
#include "physics_state.h"
#include <stdio.h>
#include <stdlib.h>

movement gen_movement(void)
{
  static movement _movement = DEFAULT_MOVEMENT;
  return _movement;
}
void free_movement(movement *_movement)
{
  free_kinematic_property(&_movement->momentum);
  free_kinematic_property(&_movement->force);
}
physics_state update_physics_state_movement
(physics_state _physics_state, movement *_movement)
/* Returns a physics_state updated by PHYSICS_STEP_S milliseconds */
{
  add_instant_kinematic_factor(&_movement->momentum,
			       update_kinematic_property(&_movement->force,
							 PHYSICS_STEP_S));
  _movement->displacement = scale_vector(update_kinematic_property(&_movement->momentum,
								   PHYSICS_STEP_S),
					 1.0 / _movement->mass);
  _physics_state.position = add_vector(_physics_state.position,
				       _movement->displacement);
  return _physics_state;
}
void set_movement_gravity
(movement *_movement, vector gravity, float scale)
{
  _movement->gravity = gravity;
  if(-1 == _movement->gravity_id){
    _movement->gravity_id = add_kinematic_factor(&_movement->force,
						    _movement->gravity,
						    scale * _movement->mass);
  } else{
    replace_kinematic_factor(&_movement->force, gravity, scale, _movement->gravity_id);
  }
}
void print_movement(movement _movement)
{
  printf("mass:%f\n", _movement.mass);
  printf("gravity id: %d with gravity:\n", _movement.gravity_id);
  print_vector(_movement.gravity);
  printf("momentum:\n");
  print_kinematic_property(_movement.momentum);
  printf("force:\n");
  print_kinematic_property(_movement.force);
}
