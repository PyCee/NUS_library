#include "NUS_multi_gpu.h"
#include "NUS_vulkan_instance.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

NUS_result nus_multi_gpu_build
(NUS_vulkan_instance vulkan_instance, NUS_multi_gpu *p_multi_gpu)
{
  unsigned int i;
  
  if(vkEnumeratePhysicalDevices(vulkan_instance.vk_instance,
				&p_multi_gpu->gpu_count, NULL) !=
     VK_SUCCESS || 0 == p_multi_gpu->gpu_count){
    printf("ERROR::failed enumerating physical devices: count = %d\n",
	   p_multi_gpu->gpu_count);
    return NUS_FAILURE;
  }
  
  VkPhysicalDevice physical_devices[p_multi_gpu->gpu_count];
  if(vkEnumeratePhysicalDevices(vulkan_instance.vk_instance,
				&p_multi_gpu->gpu_count, physical_devices) !=
     VK_SUCCESS){
    printf("ERROR::failed enumerating physical devices: physical devices\n");
    return NUS_FAILURE;
  }
  p_multi_gpu->gpus = malloc(sizeof(*p_multi_gpu->gpus)
			     * p_multi_gpu->gpu_count);
  for(i = 0; i < p_multi_gpu->gpu_count; ++i){
    if(nus_gpu_build(physical_devices[i], p_multi_gpu->gpus + i)
       != NUS_SUCCESS){
      printf("ERROR::failed in nus_build_gpu\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
void nus_multi_gpu_free(NUS_multi_gpu *p_multi_gpu)
{
  unsigned int i;
  for(i = 0; i < p_multi_gpu->gpu_count; ++i){
    nus_gpu_free(p_multi_gpu->gpus + i);
  }
  
  free(p_multi_gpu->gpus);
  p_multi_gpu->gpus = NULL;

  p_multi_gpu->gpu_count = 0;
}
void nus_multi_gpu_print(NUS_multi_gpu multi_gpu)
{
  unsigned int i;
  
  printf("printing NUS_multi_gpu:\ncontains %d gpu(s)\n", multi_gpu.gpu_count);
  for(i = 0; i < multi_gpu.gpu_count; ++i){
    nus_gpu_print(multi_gpu.gpus[i]);
  }
}
NUS_result nus_multi_gpu_check_surface_support
(VkSurfaceKHR surface, NUS_multi_gpu *p_multi_gpu)
{
  unsigned int i,
    j;
  for(i = 0; i < p_multi_gpu->gpu_count; ++i){
    for(j = 0; j < p_multi_gpu->gpus[i].queue_family_count; ++j){
      if(nus_queue_family_test_surface_support(p_multi_gpu->gpus[i].physical_device,
					       surface,
					       p_multi_gpu->gpus[i].queue_families
					       + j) != NUS_SUCCESS){
	printf("ERROR::failed testing for surface support\n");
      }
    }
  }
  return NUS_SUCCESS;
}
NUS_result nus_multi_gpu_submit_commands(NUS_multi_gpu multi_gpu)
{
  unsigned int i;
  for(i = 0; i < multi_gpu.gpu_count; ++i){
    if(nus_gpu_submit_commands(multi_gpu.gpus[i]) != NUS_SUCCESS){
      printf("ERROR::failed to submit gpu command queues\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
