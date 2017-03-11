#include "NUS_joint.h"
#include <string.h>
#include <limits.h>
#include <stdlib.h>

NUS_joint nus_joint_build
(NUS_matrix inv_bind_pose, char const *string, unsigned int parent_index)
{
  NUS_joint joint;
  joint.inv_bind_pose = inv_bind_pose;
  joint.name = malloc(strlen(string) + 1);
  strcpy(joint.name, string);
  joint.parent_index = UINT_MAX;//parent;
  return joint;
}
void nus_joint_free(NUS_joint *p_joint)
{
  free(p_joint->name);
}
