#ifndef NUS_ANIMATION_H
#define NUS_ANIMATION_H

#include "NUS_keyframe.h"
#include <stdlib.h>
#include <stdint.h>

#define NUS_ANIMATION_NAME_SIZE 64

struct NUS_skeleton;

typedef struct NUS_animation{
  struct NUS_skeleton *p_skeleton;
  char name[NUS_ANIMATION_NAME_SIZE];
  float duration;// Duration of the animation (in seconds)
  uint32_t frame_count;
  float *times;// Time of each keyframe in (0, 1) of the animations duration
  NUS_keyframe *keyframes;
} NUS_animation;

NUS_animation nus_animation_build(struct NUS_skeleton *, NUS_animation *);
void nus_animation_free(NUS_animation *);

#endif /* NUS_ANIMATION_H */
