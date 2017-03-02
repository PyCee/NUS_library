#include "NUS_physics_state.h"
#include "NUS_movement.h"
#include "NUS_orientation.h"
#include <stdio.h>

NUS_physics_state nus_physics_state_build(void)
{
  return (NUS_physics_state){
    .position = nus_vector_build(0.0, 0.0, 0.0),
      .local_orientation = nus_axes_build(nus_vector_build(0.0, 0.0, -1.0),
					  nus_vector_build(0.0, 1.0, 0.0),
					  nus_vector_build(-1.0, 0.0, 0.0))
  };
}
void nus_physics_state_free(NUS_physics_state *p_physics_state)
{
  // reset physics state
  *p_physics_state = nus_physics_state_build();
}
NUS_physics_state nus_physics_state_interpolate
(NUS_physics_state physics_state_0,
 NUS_physics_state physics_state_1, double t)
{
  t = (t < 1.0) ? t : 1.0;
  t = (t > 0.0) ? t : 0.0;

  physics_state_0.position = nus_vector_interpolate(physics_state_0.position,
						    physics_state_1.position, t);
  physics_state_0.local_orientation =
    nus_axes_interpolate(physics_state_0.local_orientation,
			 physics_state_1.local_orientation, t);
  return physics_state_0;
}
NUS_physics_state nus_physics_state_step
(NUS_physics_state physics_state, NUS_movement *p_movement,
 NUS_orientation *p_orientation)
{
  if(p_movement != NULL){
  physics_state = nus_movement_apply(p_movement, physics_state);
  }
  if(p_orientation != NULL){
  physics_state = nus_orientation_apply(p_orientation, physics_state);
  }
  return physics_state;
}
void nus_physics_state_print(NUS_physics_state physics_state)
{
  printf("physics state position: ");
  nus_vector_print(physics_state.position);
  printf("physics state orientation: ");
  nus_axes_print(physics_state.local_orientation);
}
