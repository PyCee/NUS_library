#ifndef NUS_GRAPHICS_PIPELINE_H
#define NUS_GRAPHICS_PIPELINE_H

#include "../NUS_vulkan.h"
#include "NUS_shader.h"

struct NUS_gpu;

typedef struct NUS_graphics_pipeline_info{

} NUS_graphics_pipeline_info;

typedef struct NUS_graphics_pipeline{
  NUS_shader *shaders;
  VkPipeline vk_graphics_pipeline;
} NUS_graphics_pipeline;

#endif /* NUS_GRAPHICS_PIPELINE_H */
