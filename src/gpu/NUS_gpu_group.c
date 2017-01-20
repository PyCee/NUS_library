#include "NUS_gpu_group.h"
#include <stdlib.h>
#include <stdio.h>

NUS_result nus_gpu_group_build
(VkInstance vulkan_instance, NUS_gpu_group *NUS_gpu_group_)
{
  unsigned int i;
  
  if(vkEnumeratePhysicalDevices(vulkan_instance, &NUS_gpu_group_->gpu_count, NULL) 
     != VK_SUCCESS || 0 == NUS_gpu_group_->gpu_count){
    printf("ERROR::failed enumerating physical devices\n");
    return NUS_FAILURE;
  }
  NUS_gpu_group_->physical_devices = malloc(sizeof(*NUS_gpu_group_->physical_devices)
					   * NUS_gpu_group_->gpu_count);
  if(vkEnumeratePhysicalDevices(vulkan_instance, &NUS_gpu_group_->gpu_count,
				NUS_gpu_group_->physical_devices) != VK_SUCCESS){
    printf("ERROR::failed enumerating physical devices\n");
    return NUS_FAILURE;
  }
  NUS_gpu_group_->gpus = malloc(sizeof(*NUS_gpu_group_->gpus)
				* NUS_gpu_group_->gpu_count);
  
  for(i = 0; i < NUS_gpu_group_->gpu_count; ++i){
    if(nus_gpu_build(NUS_gpu_group_->physical_devices[i], NUS_gpu_group_->gpus + i)
       != NUS_SUCCESS){
      printf("ERROR::failed in nus_build_gpu\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
void nus_gpu_group_free(NUS_gpu_group *NUS_gpu_group_)
{
  unsigned int i;
  for(i = 0; i < NUS_gpu_group_->gpu_count; ++i){
    nus_gpu_free(NUS_gpu_group_->gpus + i);
  }
  free(NUS_gpu_group_->gpus);
  NUS_gpu_group_->gpus = NULL;

  /* The actual physical devices are destroyed when the instance is destroyed */
  free(NUS_gpu_group_->physical_devices);
  NUS_gpu_group_->physical_devices = NULL;
  NUS_gpu_group_->gpu_count = 0;
}
void nus_gpu_group_print(NUS_gpu_group NUS_gpu_group_)
{
  unsigned int i;
  VkPhysicalDeviceProperties physical_device_properties;
  
  printf("printing NUS_gpu_group:\ncontains %d gpu(s)\n", NUS_gpu_group_.gpu_count);
  for(i = 0; i < NUS_gpu_group_.gpu_count; ++i){
    vkGetPhysicalDeviceProperties(NUS_gpu_group_.physical_devices[i],
				  &physical_device_properties);
    printf("gpu: %s\n", physical_device_properties.deviceName);
    nus_gpu_print(NUS_gpu_group_.gpus[i]);
  }
}
NUS_result nus_gpu_group_check_surface_support
(VkSurfaceKHR surface, NUS_gpu_group *NUS_gpu_group_)
{
  unsigned int i,
    j;
  for(i = 0; i < NUS_gpu_group_->gpu_count; ++i){
    for(j = 0; j < NUS_gpu_group_->gpus[i].family_count; ++j){
      if(nus_queue_family_test_surface_support(NUS_gpu_group_->physical_devices[i],
					       surface,
					       NUS_gpu_group_->gpus[i].queue_families
					       + j) != NUS_SUCCESS){
	printf("ERROR::failed testing for surface support\n");
      }
    }
  }
  return NUS_SUCCESS;
}
