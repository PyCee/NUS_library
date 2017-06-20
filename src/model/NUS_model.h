#ifndef NUS_MODEL_H
#define NUS_MODEL_H

#include "../NUS_result.h"
#include "NUS_vertex.h"
#include "../NUS_vulkan.h"
#include "../gpu/NUS_binding.h"
#include "../gpu/NUS_memory_map.h"
#include "../strings/NUS_absolute_path.h"
#include <NUSM/NUSM_library.h>
#include "../render/NUS_texture.h"

struct NUS_queue_info;

typedef struct NUS_model{
  NUSM_contents contents;
  NUS_memory_map vertex_memory,
    index_memory;
  NUS_texture texture;
  // texture
  // skeleton
  NUS_binding binding;
} NUS_model;

NUS_result nus_model_build(NUS_absolute_path, NUS_model *);
void nus_model_free(NUS_model *);

#endif /* NUS_MODEL_H */
