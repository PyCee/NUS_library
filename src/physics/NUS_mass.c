#include "NUS_mass.h"
#include <stdio.h>

NUS_mass nus_mass_build(NUS_component_key key)
{
  NUS_mass mass;
  if(key & NUS_COMPONENT_KEY_MOVEMENT){
    mass.movement = malloc(sizeof(*mass.movement));
    *mass.movement = nus_movement_build();
  } else{
    mass.movement = NULL;
  }
  if(key & NUS_COMPONENT_KEY_ORIENTATION){
    mass.orientation = malloc(sizeof(*mass.orientation));
    *mass.orientation = nus_orientation_build();
  } else{
    mass.orientation = NULL;
  }
  mass.prev_state = nus_physics_state_build();
  mass.next_state = nus_physics_state_build();
  mass.curr_state = nus_physics_state_build();
  mass.world_transformation = nus_matrix_identity();
  return mass;
}
void nus_mass_free(NUS_mass *p_mass)
{
  if(p_mass->movement != NULL){
    nus_movement_free(p_mass->movement);
    free(p_mass->movement);
    p_mass->movement = NULL;
  }
  if(p_mass->orientation != NULL){
    nus_orientation_free(p_mass->orientation);
    free(p_mass->orientation);
    p_mass->orientation = NULL;
  }
  nus_physics_state_free(&p_mass->prev_state);
  nus_physics_state_free(&p_mass->next_state);
  nus_physics_state_free(&p_mass->curr_state);
  p_mass->world_transformation = nus_matrix_identity();
}
void nus_mass_batch_update
(void *start, size_t offset, size_t stride, unsigned int mass_count, double delta_s)
{
  NUS_mass *p_mass;
  
  // Tracks progress made between physics states of last frame
  static double step_progress = 0;
  double t;
  unsigned int i;
  delta_s += step_progress;
  step_progress = 0;
  
  while(delta_s >= NUS_PHYSICS_STEP_S){
    for(i = 0; i < mass_count; i++){
      p_mass = (NUS_mass *)((char *)start + offset + (sizeof(*p_mass) + stride) * i);
      p_mass->prev_state = p_mass->next_state;
      p_mass->next_state = nus_physics_state_step(p_mass->prev_state,
						  p_mass->movement,
						  p_mass->orientation);
    }
    delta_s -= NUS_PHYSICS_STEP_S;
  }

  step_progress = delta_s;
  t = step_progress / NUS_PHYSICS_STEP_S;
  for(i = 0; i < mass_count; i++){
    p_mass = (NUS_mass *)((char *)start + offset + (sizeof(*p_mass) + stride) * i);
    p_mass->curr_state =
      nus_physics_state_interpolate(p_mass->prev_state,
				    p_mass->next_state, t);
    p_mass->world_transformation =
      nus_matrix_transformation(p_mass->curr_state.position,
				p_mass->curr_state.local_orientation);
  }
}
void nus_mass_print(NUS_mass mass)
{
  printf("printing mass:\ncurrent state:\n");
  nus_physics_state_print(mass.curr_state);
  printf("movement:\n");
  nus_movement_print(*mass.movement);
  printf("orientation:\n");
  nus_orientation_print(*mass.orientation);
}
