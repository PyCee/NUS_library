#include "NUS_multi_gpu.h"
#include "NUS_vulkan_instance.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "NUS_suitable_queue_info.h"

NUS_result nus_multi_gpu_build
(NUS_vulkan_instance NUS_vulkan_instance_, NUS_multi_gpu *p_multi_gpu)
{
  unsigned int i;
  
  if(vkEnumeratePhysicalDevices(NUS_vulkan_instance_.instance,
				&p_multi_gpu->gpu_count, NULL) !=
     VK_SUCCESS || 0 == p_multi_gpu->gpu_count){
    printf("ERROR::failed enumerating physical devices: count = %d\n",
	   p_multi_gpu->gpu_count);
    return NUS_FAILURE;
  }
  p_multi_gpu->physical_devices = malloc(sizeof(p_multi_gpu->physical_devices) *
					    p_multi_gpu->gpu_count);
  if(vkEnumeratePhysicalDevices(NUS_vulkan_instance_.instance,
				&p_multi_gpu->gpu_count,
			        p_multi_gpu->physical_devices) != VK_SUCCESS){
    printf("ERROR::failed enumerating physical devices: physical devices\n");
    return NUS_FAILURE;
  }
  p_multi_gpu->gpus = malloc(sizeof(*p_multi_gpu->gpus)
				* p_multi_gpu->gpu_count);
  for(i = 0; i < p_multi_gpu->gpu_count; ++i){
    if(nus_gpu_build(p_multi_gpu->physical_devices[i], p_multi_gpu->gpus + i)
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
  
  free(p_multi_gpu->physical_devices);
  
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
NUS_result nus_multi_gpu_find_suitable_queue
(NUS_multi_gpu multi_gpu, unsigned int flags,
 NUS_suitable_queue_info *info)
{
  unsigned int i;
  for(i = 0; i < multi_gpu.gpu_count; ++i){
    if(nus_gpu_find_suitable_queue(multi_gpu.gpus + i, flags,
					   info) !=
       NUS_SUCCESS){
      printf("ERROR::gpu not suitable, not really error... TODO\n");
      return NUS_FAILURE;
    }
    info->p_gpu = multi_gpu.gpus + i;
    info->gpu_index = i;
    break;
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
