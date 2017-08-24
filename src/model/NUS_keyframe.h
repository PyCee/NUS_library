#ifndef NUS_KEYFRAME_H
#define NUS_KEYFRAME_H

#include "../math/NUS_vector.h"
#include "../math/NUS_quaternion.h"

typedef struct NUS_keyframe_joint{
  NUS_quaternion rotation;
  NUS_vector scale,
    translation;
} NUS_keyframe_joint;

typedef struct NUS_keyframe{
  NUS_keyframe_joint *joints;
} NUS_keyframe;

NUS_keyframe nus_keyframe_build(unsigned int);
void nus_keyframe_free(NUS_keyframe *);
void nus_keyframe_lerp
(NUS_keyframe *, NUS_keyframe, NUS_keyframe, unsigned int, float);

#endif /* NUS_KEYFRAME_H */
