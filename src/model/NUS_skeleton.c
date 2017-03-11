#include "NUS_skeleton.h"
#include <stdlib.h>

NUS_skeleton nus_skeleton_build(void)
{
  NUS_skeleton skeleton;
  /* create joints with parameters
     
   */
  return skeleton;
}
void nus_skeleton_free(NUS_skeleton *p_skeleton)
{
  unsigned int i;
  for(i = 0; i < p_skeleton->joint_count; ++i){
    nus_joint_free(p_skeleton->joints + i);
  }
  free(p_skeleton->joints);
  p_skeleton->joints = NULL;
  p_skeleton->joint_count = 0;
}
