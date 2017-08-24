#include "NUS_animation.h"
#include "NUS_skeleton.h"
#include <string.h>
#include <stdio.h>

NUS_animation nus_animation_build
(NUS_skeleton *p_skeleton, void *data)
{
  NUS_animation animation;
  int i;
  size_t joint_data_size,
    times_data_size;
  animation.p_skeleton = p_skeleton;

  // Increment data to point to the frame_count
  data = ((char*)data) + sizeof(animation.p_skeleton);
  
  animation.frame_count = *(unsigned int*)data;

  // Increment data to point to the first time
  data = ((uint16_t*)data) + 1;
  
  times_data_size = sizeof(*animation.times) * animation.frame_count;
  animation.times = malloc(times_data_size);
  memcpy(animation.times, data, times_data_size);

  // Increment data to point to the first joint
  data = (char*)data + times_data_size;
  
  animation.keyframes = malloc(sizeof(*animation.keyframes) * animation.frame_count);
  for(i = 0; i < animation.frame_count; ++i){
    joint_data_size = sizeof(*animation.keyframes[i].joints) * p_skeleton->joint_count;
    animation.keyframes[i].joints = malloc(joint_data_size);
    memcpy(animation.keyframes[i].joints, data, joint_data_size);

    // Increment data to point to the next keyframe's joints
    data = (char*)data + joint_data_size;
  }
  return animation;
}
void nus_animation_free(NUS_animation *p_animation)
{
  int i;
  if(p_animation->frame_count){
    for(i = 0; i < p_animation->frame_count; ++i){
      free(p_animation->keyframes[i].joints);
    }
    free(p_animation->keyframes);
    free(p_animation->times);
  }
}
