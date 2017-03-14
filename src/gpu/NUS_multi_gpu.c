#include "NUS_multi_gpu.h"
#include "NUS_vulkan_instance.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

NUS_result nus_multi_gpu_build
(NUS_vulkan_instance NUS_vulkan_instance_, NUS_multi_gpu *NUS_multi_gpu_)
{
  unsigned int i;
  
  if(vkEnumeratePhysicalDevices(NUS_vulkan_instance_.instance,
				&NUS_multi_gpu_->gpu_count, NULL) !=
     VK_SUCCESS || 0 == NUS_multi_gpu_->gpu_count){
    printf("ERROR::failed enumerating physical devices: count = %d\n",
	   NUS_multi_gpu_->gpu_count);
    return NUS_FAILURE;
  }
  NUS_multi_gpu_->physical_devices = malloc(sizeof(NUS_multi_gpu_->physical_devices) *
					    NUS_multi_gpu_->gpu_count);
  if(vkEnumeratePhysicalDevices(NUS_vulkan_instance_.instance,
				&NUS_multi_gpu_->gpu_count,
			        NUS_multi_gpu_->physical_devices) != VK_SUCCESS){
    printf("ERROR::failed enumerating physical devices: physical devices\n");
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
  
  free(NUS_multi_gpu_->physical_devices);
  
  free(NUS_multi_gpu_->gpus);
  NUS_multi_gpu_->gpus = NULL;

  NUS_multi_gpu_->gpu_count = 0;
}
void nus_multi_gpu_print(NUS_multi_gpu NUS_multi_gpu_)
{
  unsigned int i;
  
  printf("printing NUS_multi_gpu:\ncontains %d gpu(s)\n", NUS_multi_gpu_.gpu_count);
  for(i = 0; i < NUS_multi_gpu_.gpu_count; ++i){
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
      if(nus_queue_family_test_surface_support(NUS_multi_gpu_->gpus[i].physical_device,
					       surface,
					       NUS_multi_gpu_->gpus[i].queue_families
					       + j) != NUS_SUCCESS){
	printf("ERROR::failed testing for surface support\n");
      }
    }
  }
  return NUS_SUCCESS;
}
NUS_result nus_multi_gpu_find_suitable_gpu_index
(NUS_multi_gpu NUS_multi_gpu_, unsigned int *gpu_index)
{
  // TODO replace with algorithm to determine suitable gpu
  *gpu_index = 0;
  return NUS_SUCCESS;
}
NUS_result nus_multi_gpu_find_suitable_gpu
(NUS_multi_gpu NUS_multi_gpu_, unsigned int flags,
 unsigned int *suitable_gpu_index)
{
  unsigned int i,
    suitable_queue_family_index = UINT_MAX;
  for(i = 0; i < NUS_multi_gpu_.gpu_count; ++i){
    if(nus_gpu_find_suitable_queue_family(NUS_multi_gpu_.gpus[i], flags,
					  &suitable_queue_family_index) !=
       NUS_SUCCESS){
      printf("ERROR::failed to find suitable gpu\n");
      return NUS_FAILURE;
    }
    if(UINT_MAX != suitable_queue_family_index){
      *suitable_gpu_index = i;
      return NUS_SUCCESS;
    }
  }
  if(UINT_MAX == suitable_queue_family_index){
    printf("ERROR::no suitable gpu found\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
NUS_result nus_multi_gpu_submit_commands(NUS_multi_gpu NUS_multi_gpu_)
{
  unsigned int i;
  for(i = 0; i < NUS_multi_gpu_.gpu_count; ++i){
    if(nus_gpu_submit_commands(NUS_multi_gpu_.gpus[i]) != NUS_SUCCESS){
      printf("ERROR::failed to submit gpu command queues\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
