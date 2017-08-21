#include "NUS_skeleton.h"
#include <stdlib.h>

NUS_skeleton nus_skeleton_build(int joint_count/* ... */)
{
  NUS_skeleton skeleton;
  skeleton.joint_count = joint_count;
  skeleton.joints = malloc(sizeof(*skeleton.joints) * joint_count);
  for(int i = 0; i < joint_count; ++i){
    /*
      skeleton.joints[i].inv_bind_pose = ;
      skeleton.joints[i].parant_index = ;
      strcpy(skeleton.joints[i].name, ...);
    */
  }
  return skeleton;
}
void nus_skeleton_free(NUS_skeleton *p_skeleton)
{
  free(p_skeleton->joints);
  p_skeleton->joints = NULL;
  p_skeleton->joint_count = 0;
}

