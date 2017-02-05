#include "NUS_gpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static NUS_result nus_gpu_create_logical_device(VkPhysicalDevice, NUS_gpu *);

NUS_result nus_gpu_build(VkPhysicalDevice physical_device, NUS_gpu *NUS_gpu_)
{
  unsigned int i;
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
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device,
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
  nus_gpu_create_logical_device(physical_device, NUS_gpu_);
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
  
  printf("gpu has %d queue families\n", NUS_gpu_.queue_family_count);
  for(i = 0; i < NUS_gpu_.queue_family_count; ++i){
    printf("queue family %d:\n", i);
    nus_queue_family_print(NUS_gpu_.queue_families[i]);
  }
  
}
static NUS_result nus_gpu_create_logical_device
(VkPhysicalDevice physical_device, NUS_gpu *NUS_gpu_)
{
  VkDeviceCreateInfo device_create_info;
  unsigned int gpu_queue_count,
    i;
  VkDeviceQueueCreateInfo queue_create_info[NUS_gpu_->queue_family_count];
  gpu_queue_count = 0;
  for(i = 0; i < NUS_gpu_->queue_family_count; ++i){
    gpu_queue_count += NUS_gpu_->queue_families[i].queue_count;
    queue_create_info[i] = NUS_gpu_->queue_families[i].queue_create_info;
  }
  device_create_info = (VkDeviceCreateInfo){
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueCreateInfoCount = gpu_queue_count,
    .pQueueCreateInfos = queue_create_info,
    .enabledLayerCount = 0,
    .ppEnabledLayerNames = NULL,
    .enabledExtensionCount = 0,
    .ppEnabledExtensionNames = NULL,
    .pEnabledFeatures = NULL
  };
  const char *exten[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };
  device_create_info.enabledExtensionCount = 1;
  device_create_info.ppEnabledExtensionNames = exten;
  device_create_info.pEnabledFeatures = NULL;
  
  if(vkCreateDevice(physical_device, &device_create_info, NULL,
		    &NUS_gpu_->logical_device) != VK_SUCCESS){
    printf("ERROR::failed to create logical device\n");
    return NUS_FAILURE;
  }
  nus_load_device_vulkan_library(NUS_gpu_->logical_device, &NUS_gpu_->functions);
  return NUS_SUCCESS;
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
    if(nus_queue_family_submit_commands(NUS_gpu_.queue_families[i],
					NUS_gpu_.logical_device) !=
       NUS_SUCCESS){
      printf("ERROR::failed to submit queue family command queues\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
