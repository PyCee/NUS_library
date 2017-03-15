#include "NUS_gpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "NUS_suitable_queue.h"

static NUS_result nus_gpu_build_logical_device(NUS_gpu *);

NUS_result nus_gpu_build(VkPhysicalDevice physical_device, NUS_gpu *p_gpu)
{
  unsigned int i;
  p_gpu->physical_device = physical_device;
  if(vkEnumerateDeviceExtensionProperties(physical_device, NULL, 
					  &p_gpu->extension_property_count, NULL)
     != VK_SUCCESS){
    printf("ERROR::failed enumerating device extensions\n");
    return NUS_FAILURE;
  }
  p_gpu->extension_properties = malloc(sizeof(*p_gpu->extension_properties)
					  * p_gpu->extension_property_count);
  if(vkEnumerateDeviceExtensionProperties(physical_device, NULL, 
					  &p_gpu->extension_property_count,
					  p_gpu->extension_properties)
     != VK_SUCCESS){
    printf("ERROR::failed enumerating device extensions\n");
    return NUS_FAILURE;
  }
  vkGetPhysicalDeviceQueueFamilyProperties(p_gpu->physical_device,
					   &p_gpu->queue_family_count, NULL);
  if(0 == p_gpu->queue_family_count){
    printf("ERROR::physical device doesn't support any queue families\n");
    return NUS_FAILURE;
  }
  p_gpu->family_properties = malloc(sizeof(*p_gpu->family_properties)
					     * p_gpu->queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device,
					   &p_gpu->queue_family_count, 
					   p_gpu->family_properties);
  p_gpu->queue_families = malloc(sizeof(*p_gpu->queue_families)
				    * p_gpu->queue_family_count);
  
  for(i = 0; i < p_gpu->queue_family_count; ++i){
    if(nus_queue_family_build(p_gpu->family_properties[i], i,
			      p_gpu->queue_families + i) != NUS_SUCCESS){
      printf("ERROR::failed to build family queue\n");
      return NUS_FAILURE;
    }
  }
  if(nus_gpu_build_logical_device(p_gpu) != NUS_SUCCESS){
    printf("ERROR::failed to build logical device\n");
    return NUS_FAILURE;
  }
  nus_bind_device_vulkan_library(p_gpu->functions);
  
  for(i = 0; i < p_gpu->queue_family_count; ++i){
    if(nus_queue_family_build_command_groups(p_gpu->logical_device,
					     p_gpu->queue_families + i) !=
       NUS_SUCCESS){
      printf("ERROR::failed to create queues\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
void nus_gpu_free(NUS_gpu *p_gpu)
{
  unsigned int i;

  for(i = 0; i < p_gpu->queue_family_count; ++i){
    nus_queue_family_free(p_gpu->queue_families + i, p_gpu->logical_device);
  }
  free(p_gpu->queue_families);
  p_gpu->queue_families = NULL;

  free(p_gpu->extension_properties);
  p_gpu->extension_properties = NULL;

  free(p_gpu->family_properties);
  p_gpu->family_properties = NULL;

  vkDeviceWaitIdle(p_gpu->logical_device);
  vkDestroyDevice(p_gpu->logical_device, NULL);
}
void nus_gpu_print(NUS_gpu gpu)
{
  unsigned int i;
  VkPhysicalDeviceProperties physical_device_properties;
  
  vkGetPhysicalDeviceProperties(gpu.physical_device,
				&physical_device_properties);
  printf("gpu: %s\n", physical_device_properties.deviceName);
  
  printf("gpu has %d queue families\n", gpu.queue_family_count);
  for(i = 0; i < gpu.queue_family_count; ++i){
    printf("queue family %d:\n", i);
    nus_queue_family_print(gpu.queue_families[i]);
  }
}
NUS_result nus_gpu_find_suitable_queue
(NUS_gpu *p_gpu, unsigned int flags,
 NUS_suitable_queue *info)
{
  unsigned int i;
  for(i = 0; i < p_gpu->queue_family_count; ++i){
    if((p_gpu->queue_families[i].flags & flags) == flags){
      if(nus_queue_family_find_suitable_queue(p_gpu->queue_families[i],
						      info) !=
	 NUS_SUCCESS){
	printf("ERROR::failed to find suitable queue\n");
	return NUS_FAILURE;
      }
      info->p_queue_family = p_gpu->queue_families + i;
      info->queue_family_index = i;
      break;
    }
  }
  info->p_gpu = p_gpu;
  return NUS_SUCCESS;
}
NUS_result nus_gpu_submit_commands(NUS_gpu gpu)
{
  unsigned int i;
  for(i = 0; i < gpu.queue_family_count; ++i){
    if(nus_queue_family_submit_commands(gpu.queue_families + i,
					gpu.logical_device) !=
       NUS_SUCCESS){
      printf("ERROR::failed to submit queue family command queues\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
static NUS_result nus_gpu_build_logical_device
(NUS_gpu *p_gpu)
{
  unsigned int i,
    j;
  VkDeviceQueueCreateInfo queue_create_info[p_gpu->queue_family_count];
  float *queue_priorities[p_gpu->queue_family_count];
  for(i = 0; i < p_gpu->queue_family_count; ++i){
    queue_priorities[i] = malloc(sizeof(*queue_priorities[i]) *
				 p_gpu->queue_families[i].command_group_count);
    for(j = 0; j < p_gpu->queue_families[i].command_group_count; ++j){
      queue_priorities[i][j] = 1.0;
    }
    queue_create_info[i] = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .queueFamilyIndex = i,
      .queueCount = p_gpu->queue_families[i].command_group_count,
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
    .queueCreateInfoCount = p_gpu->queue_family_count,
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
  p_gpu->logical_device = VK_NULL_HANDLE;
  if(vkCreateDevice(p_gpu->physical_device, &device_create_info, NULL,
		    &p_gpu->logical_device) != VK_SUCCESS){
    printf("ERROR::failed to create logical device\n");
    return NUS_FAILURE;
  }
  
  for(i = 0; i < p_gpu->queue_family_count; ++i){
    free(queue_priorities[i]);
  }
  
  nus_load_device_vulkan_library(p_gpu->logical_device, &p_gpu->functions);
  return NUS_SUCCESS;
}
