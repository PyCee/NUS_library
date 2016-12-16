#include "orientation.h"
#include "physics_state.h"

orientation gen_orientation(void)
{
  static orientation _orientation = DEFAULT_ORIENTATION;
  return _orientation;
}
void free_orientation(orientation *_orientation)
{

}
physics_state update_physics_state_orientation
(physics_state _physics_state, orientation *_orientation)
{
  return _physics_state;
}
