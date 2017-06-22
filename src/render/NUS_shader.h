#ifndef NUS_SHADERS_H
#define NUS_SHADERS_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "../strings/NUS_absolute_path.h"
#include "../gpu/NUS_binding.h"

typedef struct NUS_shader{
  VkShaderModule module;
  unsigned int stage;
  NUS_binding binding;
} NUS_shader;

NUS_result nus_shader_build(NUS_absolute_path, VkShaderStageFlagBits, NUS_shader *);
void nus_shader_free(NUS_shader *);

#endif /* NUS_SHADERS_H */
