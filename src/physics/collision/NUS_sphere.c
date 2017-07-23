#include "NUS_sphere.h"

NUS_sphere nus_sphere_build(NUS_vector center, float radius)
{
  return (NUS_sphere){.type = NUS_COLLISION_SPHERE,
      .center = center, .radius = radius};
}
