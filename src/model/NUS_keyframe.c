#include "NUS_keyframe.h"
#include <stdlib.h>
#include "../NUS_log.h"
NUS_keyframe nus_keyframe_build(unsigned int joint_count){
  NUS_keyframe keyframe;
  keyframe.joints = malloc(sizeof(*keyframe.joints) * joint_count);
  return keyframe;
}
void nus_keyframe_free(NUS_keyframe *p_keyframe)
{
  free(p_keyframe->joints);
}
void nus_keyframe_lerp
(NUS_keyframe *result, NUS_keyframe keyframe_0, NUS_keyframe keyframe_1,
 unsigned int joint_count, float t)
{
  //NUS_LOG("asd\nasd\nasd\n");
  //nus_vector_print(keyframe_1.joints[0].translation);
  for(unsigned int i = 0; i < joint_count; ++i){
    result->joints[i].rotation = nus_quaternion_lerp(keyframe_0.joints[i].rotation,
						     keyframe_1.joints[i].rotation, t);
    result->joints[i].scale = nus_vector_lerp(keyframe_0.joints[i].scale,
					      keyframe_1.joints[i].scale, t);
    result->joints[i].translation = nus_vector_lerp(keyframe_0.joints[i].translation,
						    keyframe_1.joints[i].translation, t);
  }
}
