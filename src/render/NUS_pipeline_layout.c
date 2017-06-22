#include "NUS_pipeline_layout.h"
#include "../NUS_log.h"

NUS_result nus_piepline_layout_build
(VkPipelineLayoutCreateInfo create_info, NUS_pipeline_layout *p_pipeline_layout)
{
  if(vkCreatePipelineLayout(nus_get_bound_device(), &create_info, NULL,
			    &p_pipeline_layout->vk_pipeline_layout) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to create pipeline layout\n");
    return NUS_FAILURE;
  }
  p_pipeline_layout->binding = nus_get_binding();
  return NUS_SUCCESS;
}
void nus_pipeline_layout_free(NUS_pipeline_layout *p_pipeline_layout)
{
  nus_bind_binding(&p_pipeline_layout->binding);
  
  if(p_pipeline_layout->vk_pipeline_layout != VK_NULL_HANDLE){
    vkDestroyPipelineLayout(nus_get_bound_device(),
			    p_pipeline_layout->vk_pipeline_layout, NULL);
    p_pipeline_layout->vk_pipeline_layout = VK_NULL_HANDLE;
  }
  nus_unbind_binding(&p_pipeline_layout->binding);
}
VkPipelineLayout nus_pipeline_layout_get_raw(NUS_pipeline_layout pipeline_layout)
{
  return pipeline_layout.vk_pipeline_layout;
}
