#include "NUS_gpu.h"
#include <stdlib.h>
#include <stdio.h>

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
					   &NUS_gpu_->family_count, NULL);
  if(0 == NUS_gpu_->family_count){
    printf("ERROR::physical device doesn't support any queue families\n");
    return NUS_FAILURE;
  }
  NUS_gpu_->family_properties = malloc(sizeof(*NUS_gpu_->family_properties)
					     * NUS_gpu_->family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device,
					   &NUS_gpu_->family_count, 
					   NUS_gpu_->family_properties);
  NUS_gpu_->queue_families = malloc(sizeof(*NUS_gpu_->queue_families)
				    * NUS_gpu_->family_count);
  for(i = 0; i < NUS_gpu_->family_count; ++i){
    if(nus_queue_family_build(NUS_gpu_->family_properties[i], i,
			      NUS_gpu_->queue_families + i) != NUS_SUCCESS){
      printf("ERROR::failed to build family queue\n");
      return NUS_FAILURE;
    }
  }
  nus_gpu_create_logical_device(physical_device, NUS_gpu_);
  nus_bind_device_vulkan_library(NUS_gpu_->functions);
  
  for(i = 0; i < NUS_gpu_->family_count; ++i){
    if(nus_queue_family_get_queues(NUS_gpu_->logical_device,
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

  for(i = 0; i < NUS_gpu_->family_count; ++i){
    nus_queue_family_free(NUS_gpu_->queue_families + i);
  }
  free(NUS_gpu_->queue_families);
  NUS_gpu_->queue_families = NULL;

  // do I need to free something for each property?
  free(NUS_gpu_->extension_properties);
  NUS_gpu_->extension_properties = NULL;

  // do I need to free something for each property?
  free(NUS_gpu_->family_properties);
  NUS_gpu_->family_properties = NULL;
  
  vkDestroyDevice(NUS_gpu_->logical_device, NULL);
}
void nus_gpu_print(NUS_gpu NUS_gpu_)
{
  unsigned int i;
  
  printf("gpu has %d queue families\n", NUS_gpu_.family_count);
  for(i = 0; i < NUS_gpu_.family_count; ++i){
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
  VkDeviceQueueCreateInfo queue_create_info[NUS_gpu_->family_count];
  gpu_queue_count = 0;
  for(i = 0; i < NUS_gpu_->family_count; ++i){
    gpu_queue_count += NUS_gpu_->queue_families[i].queue_count;
    queue_create_info[i] = NUS_gpu_->queue_families[i].queue_create_info;
  }
  
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.pNext = NULL;
  device_create_info.flags = 0;
  device_create_info.queueCreateInfoCount = gpu_queue_count;
  device_create_info.pQueueCreateInfos = queue_create_info;
  device_create_info.enabledLayerCount = 0;
  device_create_info.ppEnabledLayerNames = NULL;
  device_create_info.enabledExtensionCount = 0;
  device_create_info.ppEnabledExtensionNames = NULL;
  device_create_info.pEnabledFeatures = NULL;
  
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
