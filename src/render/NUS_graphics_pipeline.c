#include "NUS_graphics_pipeline.h"
#include "../NUS_log.h"

NUS_result nus_graphics_pipeline_build
(VkGraphicsPipelineCreateInfo create_info, NUS_graphics_pipeline *p_graphics_pipeline)
{
  if(vkCreateGraphicsPipelines(nus_get_bound_device(), VK_NULL_HANDLE, 1, &create_info,
			       NULL, &p_graphics_pipeline->vk_graphics_pipeline) !=
     VK_SUCCESS){
    NUS_LOG_ERROR("failed to create graphics pipeline\n");
    return NUS_FAILURE;
  }
  p_graphics_pipeline->viewport = (VkViewport){
    .x = 0,
    .y = 0,
    .width = 0,
    .height = 0,
    .minDepth = 0.0f,
    .maxDepth = 1.0f
  };
  p_graphics_pipeline->scissor = (VkRect2D){
    .offset = {
      .x = 0,
      .y = 0
    },
    .extent = {
      .width = 0,
      .height = 0
    }
  };
  p_graphics_pipeline->binding = nus_get_binding();
  return NUS_SUCCESS;
}
void nus_graphics_pipeline_free(NUS_graphics_pipeline *p_graphics_pipeline)
{
  nus_bind_binding(&p_graphics_pipeline->binding);
  
  vkDestroyPipeline(nus_get_bound_device(),
		    p_graphics_pipeline->vk_graphics_pipeline, NULL);
  p_graphics_pipeline->vk_graphics_pipeline = VK_NULL_HANDLE;
  
  nus_unbind_binding(&p_graphics_pipeline->binding);
}
void nus_cmd_graphics_pipeline_bind
(VkCommandBuffer cmd_buffer, NUS_graphics_pipeline graphics_pipeline)
{
  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		    graphics_pipeline.vk_graphics_pipeline);
  vkCmdSetViewport(cmd_buffer, 0, 1, &graphics_pipeline.viewport);
  vkCmdSetScissor(cmd_buffer, 0, 1, &graphics_pipeline.scissor);
}
void nus_graphics_pipeline_set_viewport
(VkViewport viewport, NUS_graphics_pipeline *p_graphics_pipeline)
{
  p_graphics_pipeline->viewport = viewport;
}
void nus_graphics_pipeline_set_scissor
(VkRect2D scissor, NUS_graphics_pipeline *p_graphics_pipeline)
{
  p_graphics_pipeline->scissor = scissor;
}
