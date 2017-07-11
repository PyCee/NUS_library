#include "NUS_gpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "../NUS_log.h"

static NUS_result nus_gpu_build_logical_device(NUS_gpu *);

NUS_result nus_gpu_build(VkPhysicalDevice physical_device, NUS_gpu *p_gpu)
{
  unsigned int i;
  p_gpu->physical_device = physical_device;
  if(vkEnumerateDeviceExtensionProperties(physical_device, NULL, 
					  &p_gpu->extension_property_count, NULL)
     != VK_SUCCESS){
    NUS_LOG_ERROR("failed enumerating device extensions\n");
    return NUS_FAILURE;
  }
  p_gpu->extension_properties = malloc(sizeof(*p_gpu->extension_properties)
					  * p_gpu->extension_property_count);
  if(vkEnumerateDeviceExtensionProperties(physical_device, NULL, 
					  &p_gpu->extension_property_count,
					  p_gpu->extension_properties)
     != VK_SUCCESS){
    NUS_LOG_ERROR("failed enumerating device extensions\n");
    return NUS_FAILURE;
  }

  vkGetPhysicalDeviceMemoryProperties(physical_device,
				      &p_gpu->memory_properties);
  
  vkGetPhysicalDeviceQueueFamilyProperties(p_gpu->physical_device,
					   &p_gpu->queue_family_count, NULL);
  if(0 == p_gpu->queue_family_count){
    NUS_LOG_ERROR("physical device doesn't support any queue families\n");
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
      NUS_LOG_ERROR("failed to build family queue\n");
      return NUS_FAILURE;
    }
  }
  if(nus_gpu_build_logical_device(p_gpu) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to build logical device\n");
    return NUS_FAILURE;
  }
  nus_bind_device_vulkan_library(p_gpu->functions);
  
  for(i = 0; i < p_gpu->queue_family_count; ++i){
    if(nus_queue_family_build_submission_queues(p_gpu->logical_device,
					     p_gpu->queue_families + i) !=
       NUS_SUCCESS){
      NUS_LOG_ERROR("failed to create queues\n");
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
  NUS_LOG("gpu: %s\n", physical_device_properties.deviceName);
  
  NUS_LOG("gpu has %d queue families\n", gpu.queue_family_count);
  for(i = 0; i < gpu.queue_family_count; ++i){
    NUS_LOG("queue family %d:\n", i);
    nus_queue_family_print(gpu.queue_families[i]);
  }
}
NUS_result nus_gpu_submit_commands(NUS_gpu gpu)
{
  unsigned int i;
  for(i = 0; i < gpu.queue_family_count; ++i){
    if(nus_queue_family_submit_commands(gpu.queue_families + i,
					gpu.logical_device) !=
       NUS_SUCCESS){
      NUS_LOG_ERROR("failed to submit queue family command queues\n");
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
				 p_gpu->queue_families[i].submission_queue_count);
    for(j = 0; j < p_gpu->queue_families[i].submission_queue_count; ++j){
      queue_priorities[i][j] = 1.0;
    }
    queue_create_info[i] = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .queueFamilyIndex = i,
      .queueCount = p_gpu->queue_families[i].submission_queue_count,
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
    NUS_LOG_ERROR("failed to create logical device\n");
    return NUS_FAILURE;
  }
  
  for(i = 0; i < p_gpu->queue_family_count; ++i){
    free(queue_priorities[i]);
  }
  
  nus_load_device_vulkan_library(p_gpu->logical_device, &p_gpu->functions);
  return NUS_SUCCESS;
}

unsigned int nus_gpu_memory_type_index
(NUS_gpu gpu, VkMemoryRequirements mem_req, VkMemoryPropertyFlags flags)
{
  unsigned int i;
  for(i = 0; i < gpu.memory_properties.memoryTypeCount; ++i){
    if((mem_req.memoryTypeBits & (uint32_t)(1 << i)) &&
       ((gpu.memory_properties.memoryTypes[i].propertyFlags & flags) == flags)){
      // If viable memory type has been found
      return i;
    }
  }
  NUS_LOG_ERROR("failed to find viable memory type\n");
  return UINT_MAX;
}
NUS_bool nus_gpu_qwery_format_support
(NUS_gpu gpu, VkFormat format, VkImageTiling tiling,
 VkFormatFeatureFlags features)
{
  VkFormatProperties properties;
  vkGetPhysicalDeviceFormatProperties(gpu.physical_device, format, &properties);
  if((tiling == VK_IMAGE_TILING_LINEAR &&
      (properties.linearTilingFeatures & features) == features) ||
     (tiling == VK_IMAGE_TILING_OPTIMAL &&
      (properties.optimalTilingFeatures & features) == features)){
    return NUS_TRUE;
  }
  return NUS_FALSE;
}
