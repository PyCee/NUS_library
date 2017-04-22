#include "NUS_graphics_pipeline.h"
#include "../gpu/NUS_gpu.h"
#include <stdio.h>

NUS_result nus_graphics_pipeline_build
(NUS_gpu gpu,
 VkGraphicsPipelineCreateInfo pipeline_info,
 NUS_graphics_pipeline *p_graphics_pipeline)
{
  if(vkCreateGraphicsPipelines(gpu.logical_device,
			       VK_NULL_HANDLE, 1, &pipeline_info,
			       NULL,
			       &p_graphics_pipeline->vk_graphics_pipeline) !=
     VK_SUCCESS){
    printf("ERROR::failed to create graphics pipelines\n");
    return -1;
  }
  return NUS_SUCCESS;
}
void nus_graphics_pipeline_free
(NUS_gpu gpu, NUS_graphics_pipeline *p_graphics_pipeline)
{

}
