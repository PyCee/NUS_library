#ifndef NUS_JOINT_H
#define NUS_JOINT_H

#include "../math/NUS_matrix.h"

typedef struct NUS_joint{
  NUS_matrix inv_bind_pose;
  char *name;
  unsigned int parent_index;
} NUS_joint;

NUS_joint nus_joint_build(NUS_matrix, char const *, unsigned int);
void nus_joint_free(NUS_joint *);

#endif /* NUS_JOINT_H */
