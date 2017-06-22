#ifndef NUS_PIPELINE_LAYOUT_H
#define NUS_PIPELINE_LAYOUT_H

#include "../NUS_vulkan.h"
#include "../gpu/NUS_binding.h"

typedef struct NUS_pipeline_layout{
  VkPipelineLayout vk_pipeline_layout;
  NUS_binding binding;
} NUS_pipeline_layout;

NUS_result nus_pipeline_layout_build(VkPipelineLayoutCreateInfo, NUS_pipeline_layout *);
void nus_pipeline_layout_free(NUS_pipeline_layout *);
VkPipelineLayout nus_pipeline_layout_get_raw(NUS_pipeline_layout);

#endif /* NUS_PIPELINE_LAYOUT_H */
