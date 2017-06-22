#ifndef NUS_GRAPHICS_PIPELINE_H
#define NUS_GRAPHICS_PIPELINE_H

#include "../NUS_vulkan.h"
#include "NUS_shader.h"
#include "../gpu/NUS_binding.h"

typedef struct NUS_graphics_pipeline{
  VkPipeline vk_graphics_pipeline;
  VkViewport viewport;
  VkRect2D scissor;
  NUS_binding binding;
} NUS_graphics_pipeline;

NUS_result nus_graphics_pipeline_build
(VkGraphicsPipelineCreateInfo, NUS_graphics_pipeline *);
void nus_graphics_pipeline_free(NUS_graphics_pipeline *);
void nus_cmd_graphics_pipeline_bind(VkCommandBuffer, NUS_graphics_pipeline);
void nus_graphics_pipeline_set_viewport(VkViewport, NUS_graphics_pipeline *);
void nus_graphics_pipeline_set_scissor(VkRect2D, NUS_graphics_pipeline *);

#endif /* NUS_GRAPHICS_PIPELINE_H */
