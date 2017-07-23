#include "NUS_aabb.h"


NUS_aabb nus_aabb_build(NUS_vector lowest, NUS_vector highest)
{
  return (NUS_aabb){.lowest = lowest, .highest = highest};
}
NUS_bool nus_aabb_collision_test(NUS_aabb aabb_0, NUS_aabb aabb_1)
{
  return (aabb_0.lowest.x < aabb_1.highest.x &&
	  aabb_0.highest.x > aabb_1.lowest.x &&
	  aabb_0.lowest.y < aabb_1.highest.y &&
	  aabb_0.highest.y > aabb_1.lowest.y &&
	  aabb_0.lowest.z < aabb_1.highest.z &&
	  aabb_0.highest.z > aabb_1.lowest.z);
}
