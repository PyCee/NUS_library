#ifndef NUS_FRAME_H
#define NUS_FRAME_H

#include "NUS_skeleton.h"

struct NUS_animation;

typedef struct NUS_frame{
  struct NUS_animation *p_animation;
  NUS_skeleton_pose pose;
  float progress;
} NUS_frame;

NUS_frame nus_frame_build(struct NUS_animation *);
void nus_frame_free(NUS_frame *);
void nus_frame_update(NUS_frame *, float);

#endif /* NUS_FRAME_H */
