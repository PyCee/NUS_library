#ifndef NUS_GRAPHICS_PIPELINE_H
#define NUS_GRAPHICS_PIPELINE_H

#include "../NUS_vulkan.h"
#include "NUS_shader.h"

struct NUS_gpu;
//TODO impliment this struct or a version of it
typedef struct NUS_graphics_pipeline{
  NUS_shader *shaders;
  VkPipeline vk_graphics_pipeline;
  VkPipelineLayout vk_pipeline_layout;
  unsigned int shader_count;
} NUS_graphics_pipeline;

NUS_result nus_graphics_pipeline_build
(struct NUS_gpu, VkGraphicsPipelineCreateInfo, NUS_graphics_pipeline *);
void nus_graphics_pipeline_free(struct NUS_gpu, NUS_graphics_pipeline *);

#endif /* NUS_GRAPHICS_PIPELINE_H */
