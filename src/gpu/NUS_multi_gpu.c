#include "NUS_multi_gpu.h"
#include <stdlib.h>
#include <stdio.h>

NUS_result nus_multi_gpu_build
(VkInstance vulkan_instance, NUS_multi_gpu *NUS_multi_gpu_)
{
  unsigned int i;
  
  if(vkEnumeratePhysicalDevices(vulkan_instance, &NUS_multi_gpu_->gpu_count, NULL) 
     != VK_SUCCESS || 0 == NUS_multi_gpu_->gpu_count){
    printf("ERROR::failed enumerating physical devices\n");
    return NUS_FAILURE;
  }
  NUS_multi_gpu_->physical_devices = malloc(sizeof(*NUS_multi_gpu_->physical_devices)
					   * NUS_multi_gpu_->gpu_count);
  if(vkEnumeratePhysicalDevices(vulkan_instance, &NUS_multi_gpu_->gpu_count,
				NUS_multi_gpu_->physical_devices) != VK_SUCCESS){
    printf("ERROR::failed enumerating physical devices\n");
    return NUS_FAILURE;
  }
  NUS_multi_gpu_->gpus = malloc(sizeof(*NUS_multi_gpu_->gpus)
				* NUS_multi_gpu_->gpu_count);
  
  for(i = 0; i < NUS_multi_gpu_->gpu_count; ++i){
    if(nus_gpu_build(NUS_multi_gpu_->physical_devices[i], NUS_multi_gpu_->gpus + i)
       != NUS_SUCCESS){
      printf("ERROR::failed in nus_build_gpu\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
void nus_multi_gpu_free(NUS_multi_gpu *NUS_multi_gpu_)
{
  unsigned int i;
  for(i = 0; i < NUS_multi_gpu_->gpu_count; ++i){
    nus_gpu_free(NUS_multi_gpu_->gpus + i);
  }
  free(NUS_multi_gpu_->gpus);
  NUS_multi_gpu_->gpus = NULL;

  /* The actual physical devices are destroyed when the instance is destroyed */
  free(NUS_multi_gpu_->physical_devices);
  NUS_multi_gpu_->physical_devices = NULL;
  NUS_multi_gpu_->gpu_count = 0;
}
void nus_multi_gpu_print(NUS_multi_gpu NUS_multi_gpu_)
{
  unsigned int i;
  VkPhysicalDeviceProperties physical_device_properties;
  
  printf("printing NUS_multi_gpu:\ncontains %d gpu(s)\n", NUS_multi_gpu_.gpu_count);
  for(i = 0; i < NUS_multi_gpu_.gpu_count; ++i){
    vkGetPhysicalDeviceProperties(NUS_multi_gpu_.physical_devices[i],
				  &physical_device_properties);
    printf("gpu: %s\n", physical_device_properties.deviceName);
    nus_gpu_print(NUS_multi_gpu_.gpus[i]);
  }
}
NUS_result nus_multi_gpu_check_surface_support
(VkSurfaceKHR surface, NUS_multi_gpu *NUS_multi_gpu_)
{
  unsigned int i,
    j;
  for(i = 0; i < NUS_multi_gpu_->gpu_count; ++i){
    for(j = 0; j < NUS_multi_gpu_->gpus[i].queue_family_count; ++j){
      if(nus_queue_family_test_surface_support(NUS_multi_gpu_->physical_devices[i],
					       surface,
					       NUS_multi_gpu_->gpus[i].queue_families
					       + j) != NUS_SUCCESS){
	printf("ERROR::failed testing for surface support\n");
      }
    }
  }
  return NUS_SUCCESS;
}

NUS_result nus_multi_gpu_find_suitable_queue
(NUS_multi_gpu NUS_multi_gpu_, unsigned int flags, VkQueue *queue)
{
  unsigned int i,
    suitable_queue_found = 0;
  for(i = 0; i < NUS_multi_gpu_.gpu_count; ++i){
    if(nus_gpu_find_suitable_queue(NUS_multi_gpu_.gpus[i],
				   flags, &suitable_queue_found,
				   queue) != NUS_SUCCESS){
      printf("ERROR::failed to find gpu suitable queue\n");
      return NUS_FAILURE;
    }
  }
  if(0 == suitable_queue_found){
    printf("ERROR::no suitable queue found\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
