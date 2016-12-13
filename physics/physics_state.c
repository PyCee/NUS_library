#include "physics_state.h"
#include <stdio.h>
#include "movement.h"
#include "orientation.h"

physics_state gen_physics_state(void)
{
  static physics_state _physics_state = DEFAULT_PHYSICS_STATE;
  return _physics_state;
}
void free_physics_state(physics_state *_physics_state)
{
}
physics_state interpolate_physics_states
(physics_state _physics_state1, physics_state _physics_state2, float t)
{
  t = (t < 1.0) ? t : 1.0;
  t = (t > 0.0) ? t : 0.0;

  _physics_state1.local_orientation =
    interpolate_cartesian_system(_physics_state1.local_orientation,
				 _physics_state2.local_orientation, t);
  _physics_state1.head_orientation =
    interpolate_cartesian_system(_physics_state1.head_orientation,
				 _physics_state2.head_orientation, t);
  _physics_state1.position = interpolate_vector(_physics_state1.position,
						_physics_state2.position, t);
  return _physics_state1;
}
physics_state step_physics_state
(physics_state _physics_state, movement *_movement, orientation *_orientation)
{
  _physics_state = update_physics_state_orientation(_physics_state, _orientation);
  _physics_state = update_physics_state_movement(_physics_state, _movement);
  return _physics_state;
}
void print_physics_state(physics_state _physics_state)
{
  printf("physics_state position:\n");
  print_vector(_physics_state.position);
  printf("physics_state local forward:\n");
  print_vector(_physics_state.local_orientation.forward);
}
