#include "center.h"
#include <stdio.h>

center gen_center(void)
{
  static center _center = DEFAULT_CENTER;
  return _center;
}
void free_center(center *_center)
{
  free_movement(&_center->move);
  free_orientation(&_center->orient);
}

void batch_update_center
(void *start, size_t offset, size_t stride, short num, double delta_s)
{
  center *_center;
  static double step_progress = 0;// tracks progress made between physics states of last frame
  float t;
  short i;
  delta_s += step_progress;
  step_progress = 0;
  
  while(delta_s >= PHYSICS_STEP_S){
    for(i = 0; i < num; i++){
      _center = (center *)((char *)start + offset + (sizeof(*_center) + stride) * i);
      _center->prev_state = _center->next_state;
      _center->next_state = step_physics_state(_center->prev_state,
					       &_center->move,
					       &_center->orient);
    }
    delta_s -= PHYSICS_STEP_S;
  }

  step_progress = delta_s;
  t = step_progress / PHYSICS_STEP_S;
  for(i = 0; i < num; i++){
    _center = (center *)((char *)start + offset + (sizeof(*_center) + stride) * i);
    _center->curr_state =
      interpolate_physics_states(_center->prev_state,
				 _center->next_state, t);
    _center->world_transformation =
      build_transformation_matrix(_center->curr_state.local_orientation,
				  _center->curr_state.position);
  }
}
void set_mass(center *_center, float mass)
{
  _center->move.mass = mass;
}
void set_directional_gravity(center *_center, vector dir)
{
  set_movement_gravity(&_center->move, dir, GRAVITY_FORCE);
}
void print_center(center _center)
{
  printf("center:\n");
  print_physics_state(_center.curr_state);
  print_movement(_center.move);
}
