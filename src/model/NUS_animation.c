#include "NUS_animation.h"
#include "NUS_skeleton.h"
#include <string.h>
#include <stdio.h>
#include "../NUS_log.h"

NUS_animation nus_animation_build
(NUS_skeleton *p_skeleton, NUS_animation *p_animation)
{
  NUS_animation animation;
  int i;
  size_t joint_data_size,
    times_data_size;
  animation.p_skeleton = p_skeleton;

  memcpy(animation.name, p_animation->name, NUS_ANIMATION_NAME_SIZE);
  animation.duration = p_animation->duration;
  animation.frame_count = p_animation->frame_count;

  // Get frame times
  times_data_size = sizeof(*animation.times) * animation.frame_count;
  animation.times = malloc(times_data_size);
  memcpy(animation.times, p_animation->times, times_data_size);

  // Get all keyframe joint data
  animation.keyframes = malloc(sizeof(*animation.keyframes) * animation.frame_count);
  
  joint_data_size = sizeof(*animation.keyframes[0].joints) * p_skeleton->joint_count;
  for(i = 0; i < animation.frame_count; ++i){
    // For each frame
    // Get frame data (of all joints)
    animation.keyframes[i].joints = malloc(joint_data_size);
    memcpy(animation.keyframes[i].joints, p_animation->keyframes[i].joints,
	   joint_data_size);
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
