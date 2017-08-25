#ifndef NUS_MODEL_H
#define NUS_MODEL_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "../gpu/NUS_binding.h"
#include "../gpu/NUS_memory_map.h"
#include "../strings/NUS_absolute_path.h"
#include "../render/NUS_texture.h"
#include "NUS_skeleton.h"
#include "NUS_animation.h"

struct NUS_queue_info;

typedef struct NUS_model{
  NUS_memory_map vertex_memory,
    index_memory;
  NUS_texture texture;
  NUS_skeleton skeleton;
  NUS_animation *animations;
  unsigned int has_texture,
    has_skeleton,
    animation_count;
  NUS_binding binding;
} NUS_model;

NUS_result nus_model_build(NUS_absolute_path, NUS_model *);
void nus_model_free(NUS_model *);

#endif /* NUS_MODEL_H */
