#include "NUS_point.h"

NUS_point nus_point_build(NUS_vector position)
{
  return (NUS_point){.type = NUS_COLLISION_POINT, .position = position};
}
