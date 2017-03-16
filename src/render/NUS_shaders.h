#ifndef NUS_SHADERS_H
#define NUS_SHADERS_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"

struct NUS_gpu;

typedef struct NUS_shader{
  VkShaderModule module;
} NUS_shader;

NUS_result nus_shader_build(struct NUS_gpu, char *, NUS_shader *);
NUS_result nus_shader_build_source(struct NUS_gpu, char *, NUS_shader *);
void nus_shader_free(struct NUS_gpu ,NUS_shader *);

#endif /* NUS_SHADERS_H */
