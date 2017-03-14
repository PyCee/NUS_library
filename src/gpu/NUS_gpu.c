#include "NUS_gpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static NUS_result nus_gpu_build_logical_device(NUS_gpu *);

NUS_result nus_gpu_build(VkPhysicalDevice physical_device, NUS_gpu *NUS_gpu_)
{
  unsigned int i;
  NUS_gpu_->physical_device = physical_device;
  if(vkEnumerateDeviceExtensionProperties(physical_device, NULL, 
					  &NUS_gpu_->extension_property_count, NULL)
     != VK_SUCCESS){
    printf("ERROR::failed enumerating device extensions\n");
    return NUS_FAILURE;
  }
  NUS_gpu_->extension_properties = malloc(sizeof(*NUS_gpu_->extension_properties)
					  * NUS_gpu_->extension_property_count);
  if(vkEnumerateDeviceExtensionProperties(physical_device, NULL, 
					  &NUS_gpu_->extension_property_count,
					  NUS_gpu_->extension_properties)
     != VK_SUCCESS){
    printf("ERROR::failed enumerating device extensions\n");
    return NUS_FAILURE;
  }
  vkGetPhysicalDeviceQueueFamilyProperties(NUS_gpu_->physical_device,
					   &NUS_gpu_->queue_family_count, NULL);
  if(0 == NUS_gpu_->queue_family_count){
    printf("ERROR::physical device doesn't support any queue families\n");
    return NUS_FAILURE;
  }
  NUS_gpu_->family_properties = malloc(sizeof(*NUS_gpu_->family_properties)
					     * NUS_gpu_->queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device,
					   &NUS_gpu_->queue_family_count, 
					   NUS_gpu_->family_properties);
  NUS_gpu_->queue_families = malloc(sizeof(*NUS_gpu_->queue_families)
				    * NUS_gpu_->queue_family_count);
  
  for(i = 0; i < NUS_gpu_->queue_family_count; ++i){
    if(nus_queue_family_build(NUS_gpu_->family_properties[i], i,
			      NUS_gpu_->queue_families + i) != NUS_SUCCESS){
      printf("ERROR::failed to build family queue\n");
      return NUS_FAILURE;
    }
  }
  if(nus_gpu_build_logical_device(NUS_gpu_) != NUS_SUCCESS){
    printf("ERROR::failed to build logical device\n");
    return NUS_FAILURE;
  }
  nus_bind_device_vulkan_library(NUS_gpu_->functions);
  
  for(i = 0; i < NUS_gpu_->queue_family_count; ++i){
    if(nus_queue_family_build_queues(NUS_gpu_->logical_device,
				     NUS_gpu_->queue_families + i) != NUS_SUCCESS){
      printf("ERROR::failed to create queues\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
void nus_gpu_free(NUS_gpu *NUS_gpu_)
{
  unsigned int i;

  for(i = 0; i < NUS_gpu_->queue_family_count; ++i){
    nus_queue_family_free(NUS_gpu_->queue_families + i, NUS_gpu_->logical_device);
  }
  free(NUS_gpu_->queue_families);
  NUS_gpu_->queue_families = NULL;

  free(NUS_gpu_->extension_properties);
  NUS_gpu_->extension_properties = NULL;

  free(NUS_gpu_->family_properties);
  NUS_gpu_->family_properties = NULL;

  vkDeviceWaitIdle(NUS_gpu_->logical_device);
  vkDestroyDevice(NUS_gpu_->logical_device, NULL);
}
void nus_gpu_print(NUS_gpu NUS_gpu_)
{
  unsigned int i;
  VkPhysicalDeviceProperties physical_device_properties;
  
  vkGetPhysicalDeviceProperties(NUS_gpu_.physical_device,
				&physical_device_properties);
  printf("gpu: %s\n", physical_device_properties.deviceName);
  
  printf("gpu has %d queue families\n", NUS_gpu_.queue_family_count);
  for(i = 0; i < NUS_gpu_.queue_family_count; ++i){
    printf("queue family %d:\n", i);
    nus_queue_family_print(NUS_gpu_.queue_families[i]);
  }
}
NUS_result nus_gpu_find_suitable_queue_family
(NUS_gpu NUS_gpu_, unsigned int flags,
 unsigned int *queue_family_index)
{
  unsigned int i,
    command_buffer_count = UINT_MAX,
    queue_index = UINT_MAX;

  for(i = 0; i < NUS_gpu_.queue_family_count; ++i){
    if((NUS_gpu_.queue_families[i].flags & flags) == flags){
      if(nus_queue_family_find_suitable_queue(NUS_gpu_.queue_families[i],
					      &queue_index) !=
	 NUS_SUCCESS){
	printf("ERROR::failed to find suitable queue\n");
	return NUS_FAILURE;
      }
      if(NUS_gpu_.queue_families[i].queues[queue_index].command_buffer_count <
	 command_buffer_count){
	*queue_family_index = i;
	command_buffer_count =
	  NUS_gpu_.queue_families[i].queues[queue_index].command_buffer_count;
      }
    }
  }
  return NUS_SUCCESS;
}
NUS_result nus_gpu_add_command_buffer
(NUS_gpu NUS_gpu_, unsigned int *queue_family_index,
 VkCommandBuffer *command_buffer)
{
  
  return NUS_SUCCESS;
}
NUS_result nus_gpu_submit_commands(NUS_gpu NUS_gpu_)
{
  unsigned int i;
  for(i = 0; i < NUS_gpu_.queue_family_count; ++i){
    if(nus_queue_family_submit_commands(NUS_gpu_.queue_families + i,
					NUS_gpu_.logical_device) !=
       NUS_SUCCESS){
      printf("ERROR::failed to submit queue family command queues\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
static NUS_result nus_gpu_build_logical_device
(NUS_gpu *NUS_gpu_)
{
  unsigned int i,
    j;
  VkDeviceQueueCreateInfo queue_create_info[NUS_gpu_->queue_family_count];
  float *queue_priorities[NUS_gpu_->queue_family_count];
  for(i = 0; i < NUS_gpu_->queue_family_count; ++i){
    queue_priorities[i] = malloc(sizeof(*queue_priorities[i]) *
				 NUS_gpu_->queue_families[i].queue_count);
    for(j = 0; j < NUS_gpu_->queue_families[i].queue_count; ++j){
      queue_priorities[i][j] = 1.0;
    }
    queue_create_info[i] = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .queueFamilyIndex = i,
      .queueCount = NUS_gpu_->queue_families[i].queue_count,
      .pQueuePriorities = queue_priorities[i]
    };
  }
  const char *extensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };
  VkDeviceCreateInfo device_create_info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueCreateInfoCount = NUS_gpu_->queue_family_count,
    .pQueueCreateInfos = queue_create_info,
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = NULL,
    .enabledExtensionCount = 1,
    .ppEnabledExtensionNames = extensions,
    .pEnabledFeatures = NULL
  };
#if defined(NUS_DEBUG)
  // enable validation layer for logical device
  const char *layers[] = {
    "VK_LAYER_LUNARG_standard_validation"
  };
  device_create_info.enabledLayerCount = 1;
  device_create_info.ppEnabledLayerNames = layers;
#endif
  NUS_gpu_->logical_device = VK_NULL_HANDLE;
  if(vkCreateDevice(NUS_gpu_->physical_device, &device_create_info, NULL,
		    &NUS_gpu_->logical_device) != VK_SUCCESS){
    printf("ERROR::failed to create logical device\n");
    return NUS_FAILURE;
  }
  
  for(i = 0; i < NUS_gpu_->queue_family_count; ++i){
    free(queue_priorities[i]);
  }
  
  nus_load_device_vulkan_library(NUS_gpu_->logical_device, &NUS_gpu_->functions);
  return NUS_SUCCESS;
}
