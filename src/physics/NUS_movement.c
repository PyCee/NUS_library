#include "NUS_movement.h"
#include "NUS_physics_state.h"
#include <limits.h>
#include <stdio.h>

NUS_movement nus_movement_build(void)
{
  return (NUS_movement) {
    .momentum = nus_kinematic_property_build(),
      .force = nus_kinematic_property_build(),
      .displacement = nus_vector_build(0.0, 0.0, 0.0),
      .gravity_id = UINT_MAX,
      .mass = 1
      };
}
void nus_movement_free(NUS_movement *p_movement)
{
  nus_kinematic_property_free(&p_movement->momentum);
  nus_kinematic_property_free(&p_movement->force);
}
struct NUS_physics_state nus_movement_apply
(NUS_movement *p_movement, NUS_physics_state physics_state)
{
  nus_kinematic_property_impulse(&p_movement->momentum,
				 nus_kinematic_property_integral(&p_movement->force,
								 NUS_PHYSICS_STEP_S));
  p_movement->displacement =
    nus_vector_scale(nus_kinematic_property_integral(&p_movement->momentum,
						     NUS_PHYSICS_STEP_S),
		     1.0 / p_movement->mass);
  physics_state.position = nus_vector_add(physics_state.position,
					  p_movement->displacement);
  return physics_state;
}
void nus_movement_set_mass(NUS_movement *p_movement, unsigned int mass)
{
  p_movement->mass = mass;
  if(p_movement->gravity_id != UINT_MAX){
    NUS_vector gravity = nus_vector_scale(gravity, NUS_GRAVITY_FORCE *
					  p_movement->mass);
    nus_kinematic_property_set_factor(&p_movement->force, p_movement->gravity_id,
				      gravity);
  }
}
void nus_movement_set_gravity(NUS_movement *p_movement, NUS_vector gravity)
{
  gravity = nus_vector_scale(gravity, NUS_GRAVITY_FORCE * p_movement->mass);
  if(p_movement->gravity_id != UINT_MAX){
    nus_kinematic_property_set_factor(&p_movement->force, p_movement->gravity_id,
				      gravity);
  } else{
    printf("gravity: ");
    nus_vector_print(gravity);
    p_movement->gravity_id = nus_kinematic_property_append_factor(&p_movement->force,
								  gravity);
  }
}
void nus_movement_print(NUS_movement movement)
{
  printf("movement has mass %d, displacement: ", movement.mass);
  nus_vector_print(movement.displacement);
  printf("momentum: \n");
  nus_kinematic_property_print(movement.momentum);
  printf("force: \n");
  nus_kinematic_property_print(movement.force);
}
