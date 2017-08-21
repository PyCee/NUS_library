#ifndef NUS_BINARY_MODEL_H
#define NUS_BINARY_MODEL_H

#include <stdlib.h>
#include "NUS_skeleton.h"
#include "NUS_pose_skeleton.h"
#include "NUS_keyframe.h"

typedef struct NUS_binary_model_vertex{
  float position[3],
    normal[3],
    tex_coords[2],
    bone_weight[4];
  int bone_indices[4];
} NUS_binary_model_vertex;
#define NUS_BINARY_MODEL_VERTEX_SIZE sizeof(NUS_binary_model_vertex)

#define NUS_BINARY_MODEL_INDEX_SIZE sizeof(unsigned int)

typedef struct NUS_binary_model_animation{
  NUS_keyframe_joint *p_keyframe_joints;
  unsigned int *p_times,
    keyframe_count;
} NUS_binary_model_animation;
#define NUS_BINARY_MODEL_JOINT_SIZE sizeof(NUS_keyframe_joint)

#define NUS_BINARY_MODEL_SIZE_COUNT 8
// Number of size_t at the beginning of NUS_binary_model

typedef struct NUS_binary_model{
  size_t vertex_data_size,
    index_data_size,
    texture_width,
    texture_height,
    texture_data_size,
    joint_count,
    skeleton_data_size,
    animation_count;

  union{
    NUS_binary_model_vertex *vertices;
    void *vertex_data;
  };
  union{
    unsigned int *indices;
    void *index_data;
  };
  void *texture_data;
  union{
    NUS_skeleton_joint *skeleton_joints;
    void *skeleton_data;
  };
  NUS_binary_model_animation *p_animations;
  
} NUS_binary_model;

void nus_binary_model_free(NUS_binary_model *);

#endif /* NUS_BINARY_MODEL_H */
