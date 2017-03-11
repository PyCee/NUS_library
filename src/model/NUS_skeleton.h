#ifndef NUS_SKELETON_H
#define NUS_SKELETON_H

#include "NUS_joint.h"

typedef struct NUS_skeleton{
  NUS_joint *joints;
  unsigned int joint_count;
} NUS_skeleton;

NUS_skeleton nus_skeleton_build(void);
void nus_skeleton_free(NUS_skeleton *);

#endif /* NUS_SKELETON_H */
