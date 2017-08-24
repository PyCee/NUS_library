#include "NUS_skeleton.h"
#include <stdlib.h>
#include <string.h>

NUS_skeleton nus_skeleton_build(void *data)
{
  NUS_skeleton skeleton;
  size_t joint_data_size;
  skeleton.joint_count = *(uint32_t*)data;
  joint_data_size = sizeof(*skeleton.joints) * skeleton.joint_count;
  skeleton.joints = malloc(joint_data_size);
  memcpy(skeleton.joints, data, joint_data_size);
  return skeleton;
}
void nus_skeleton_free(NUS_skeleton *p_skeleton)
{
  free(p_skeleton->joints);
  p_skeleton->joints = NULL;
  p_skeleton->joint_count = 0;
}

