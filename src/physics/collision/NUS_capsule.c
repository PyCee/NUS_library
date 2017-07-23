#include "NUS_capsule.h"

NUS_capsule nus_capsule_build(NUS_vector p0, NUS_vector p1, float radius)
{
  return (NUS_capsule){.type = NUS_COLLISION_CAPSULE,
      .position_0 = p0, .position_1 = p1, .radius = radius};
}
