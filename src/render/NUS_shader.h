#ifndef NUS_SHADERS_H
#define NUS_SHADERS_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "../strings/NUS_absolute_path.h"

struct NUS_gpu;

typedef struct NUS_shader{
  VkShaderModule module;
  unsigned int stage;
} NUS_shader;

NUS_result nus_shader_build
(struct NUS_gpu, NUS_absolute_path, unsigned int, NUS_shader *);
void nus_shader_free(struct NUS_gpu ,NUS_shader *);

#endif /* NUS_SHADERS_H */
