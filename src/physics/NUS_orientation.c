#include "NUS_orientation.h"
#include "NUS_physics_state.h"
#include <stdio.h>

NUS_orientation nus_orientation_build(void)
{
  return (NUS_orientation){
    .target = nus_vector_build(0.0, 0.0, 0.0),
      .mode = NUS_ORIENTATION_MODE_FREE
  };
}
void nus_orientation_free(NUS_orientation *p_orientation)
{
  *p_orientation = nus_orientation_build();
}
NUS_physics_state nus_orientation_apply
(NUS_orientation *orientation, NUS_physics_state physics_state)
{
  // rotate physics_state.local_orientation according to orientation.target and .mode
  
  return physics_state;
}
void nus_orientation_print(NUS_orientation orientation)
{
  printf("TODO: print orientation\n");
}
