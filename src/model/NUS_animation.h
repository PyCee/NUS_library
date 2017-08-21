#ifndef NUS_ANIMATION_H
#define NUS_ANIMATION_H

#include "NUS_keyframe.h"
#include <stdlib.h>

struct NUS_skeleton;

typedef struct NUS_animation{
  struct NUS_skeleton *p_skeleton;
  NUS_keyframe *keyframes;
  unsigned int frame_count;
} NUS_animation;

NUS_animation nus_animation_build(unsigned int, void *, size_t);

#endif /* NUS_ANIMATION_H */
