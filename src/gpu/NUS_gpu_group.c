#include "NUS_gpu_group.h"
#include <stdlib.h>
#include <stdio.h>


static NUS_result nus_build_gpu(VkPhysicalDevice, NUS_gpu *);
static NUS_result nus_build_queue_family
(VkQueueFamilyProperties, NUS_gpu_queue_family *, unsigned int);
static NUS_result nus_build_gpu_logical_device(VkPhysicalDevice, NUS_gpu *);
static NUS_result nus_get_gpu_queues(VkDevice, NUS_gpu *);

NUS_result nus_build_gpu_group
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
    if(nus_build_gpu(NUS_gpu_group_->physical_devices[i], NUS_gpu_group_->gpus + i)
       != NUS_SUCCESS){
      printf("failed in nus_build_gpu\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
void nus_free_gpu_group(NUS_gpu_group *NUS_gpu_group_)
{

}
void nus_print_gpu_group(NUS_gpu_group NUS_gpu_group_)
{
  unsigned int i,
    j;
  VkPhysicalDeviceProperties physical_device_properties;
  
  
  printf("printing NUS_gpu_group:\ncontains %d gpu(s)\n", NUS_gpu_group_.gpu_count);
  for(i = 0; i < NUS_gpu_group_.gpu_count; ++i){
    vkGetPhysicalDeviceProperties(NUS_gpu_group_.physical_devices[i],
				  &physical_device_properties);
    printf("\tgpu %d: %s\n", i, physical_device_properties.deviceName);
    for(j = 0; j < NUS_gpu_group_.gpus[i].queue_family_count; ++j){
      printf("\t\tqueue family %d supported operations:\n", j);
      printf("\t\tgraphics %d\n",
	     NUS_gpu_group_.gpus[i].queue_families[j].flags &
	     NUS_QUEUE_SUPPORT_GRAPHICS && 1);
      printf("\t\tcompute %d\n",
	     NUS_gpu_group_.gpus[i].queue_families[j].flags &
	     NUS_QUEUE_SUPPORT_COMPUTE && 1);
      printf("\t\ttransfer %d\n",
	     NUS_gpu_group_.gpus[i].queue_families[j].flags &
	     NUS_QUEUE_SUPPORT_TRANSFER && 1);
      printf("\t\tsparse %d\n",
	     NUS_gpu_group_.gpus[i].queue_families[j].flags &
	     NUS_QUEUE_SUPPORT_SPARSE && 1);
      printf("\t\tpresent %d\n",
	     NUS_gpu_group_.gpus[i].queue_families[j].flags &
	     NUS_QUEUE_SUPPORT_PRESENT && 1);
    }
  }
}
NUS_result nus_check_gpu_presentation_support
(VkSurfaceKHR vulkan_surface, NUS_gpu_group *NUS_gpu_group_)
{
  NUS_result supported;
  unsigned int device_support,
    i,
    j;

  supported = NUS_FAILURE;
  for(i = 0; i < NUS_gpu_group_->gpu_count; ++i){
    for(j = 0; j < NUS_gpu_group_->gpus[i].queue_family_count; ++j){
      vkGetPhysicalDeviceSurfaceSupportKHR(NUS_gpu_group_->physical_devices[i],
					   j, vulkan_surface, &device_support);
      if(VK_TRUE == device_support){
	supported = NUS_SUCCESS;
	NUS_gpu_group_->gpus[i].queue_families[j].flags |= NUS_QUEUE_SUPPORT_PRESENT;
      }
    }
  }
  return supported;
}
static NUS_result nus_build_gpu(VkPhysicalDevice physical_device, NUS_gpu *NUS_gpu_)
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
					   &NUS_gpu_->queue_family_count, 
					   NULL);
  if(0 == NUS_gpu_->queue_family_count){
    printf("ERROR::physical device doesn't support any queue families\n");
    return NUS_FAILURE;
  }
  NUS_gpu_->queue_family_properties = malloc(sizeof(NUS_gpu_->queue_family_properties)
					     * NUS_gpu_->queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device,
					   &NUS_gpu_->queue_family_count, 
					   NUS_gpu_->queue_family_properties);
  NUS_gpu_->queue_families = malloc(sizeof(NUS_gpu_->queue_families)
				    * NUS_gpu_->queue_family_count);
  for(i = 0; i < NUS_gpu_->queue_family_count; ++i){
    if(nus_build_queue_family(NUS_gpu_->queue_family_properties[i], 
			      NUS_gpu_->queue_families + i, i) != NUS_SUCCESS){
      printf("ERROR::failed to build family queue\n");
      return NUS_FAILURE;
    }
  }
  if(nus_build_gpu_logical_device(physical_device, NUS_gpu_) != NUS_SUCCESS){
    printf("ERROR::failed creating gpu logical device\n");
    return NUS_FAILURE;
  }
  nus_get_gpu_queues(NUS_gpu_->device, NUS_gpu_);
  return NUS_SUCCESS;
}

static NUS_result nus_build_queue_family
(VkQueueFamilyProperties queue_family_properties, 
 NUS_gpu_queue_family *NUS_gpu_queue_family_, unsigned int queue_family_index)
{
  unsigned char i;
  float default_priority;
  
  if(0 == queue_family_properties.queueCount){
    printf("ERROR::no queues found in queue family\n");
    return NUS_FAILURE;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT){
    NUS_gpu_queue_family_->flags |= NUS_QUEUE_SUPPORT_GRAPHICS;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT){
    NUS_gpu_queue_family_->flags |= NUS_QUEUE_SUPPORT_COMPUTE;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT){
    NUS_gpu_queue_family_->flags |= NUS_QUEUE_SUPPORT_TRANSFER;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT){
    NUS_gpu_queue_family_->flags |= NUS_QUEUE_SUPPORT_SPARSE;
  }
  NUS_gpu_queue_family_->queue_count = queue_family_properties.queueCount; 
  NUS_gpu_queue_family_->queues = malloc(sizeof(*NUS_gpu_queue_family_->queues)
				       * NUS_gpu_queue_family_->queue_count);
  NUS_gpu_queue_family_->queue_family_priorities =
    malloc(sizeof(*NUS_gpu_queue_family_->queue_family_priorities) *
	   NUS_gpu_queue_family_->queue_count);
  default_priority = 1.0f / (float)NUS_gpu_queue_family_->queue_count;
  for(i = 0; i < NUS_gpu_queue_family_->queue_count; ++i){
    NUS_gpu_queue_family_->queue_family_priorities[i] = default_priority;
  }
  NUS_gpu_queue_family_->queue_create_info.sType =
    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  NUS_gpu_queue_family_->queue_create_info.pNext = NULL;
  NUS_gpu_queue_family_->queue_create_info.flags = 0;
  NUS_gpu_queue_family_->queue_create_info.queueFamilyIndex =
    queue_family_index;
  NUS_gpu_queue_family_->queue_create_info.queueCount =
    NUS_gpu_queue_family_->queue_count;
  NUS_gpu_queue_family_->queue_create_info.pQueuePriorities =
    NUS_gpu_queue_family_->queue_family_priorities;
  return NUS_SUCCESS;
}
static NUS_result nus_build_gpu_logical_device
(VkPhysicalDevice physical_device, NUS_gpu *NUS_gpu_)
{
  VkDeviceCreateInfo device_create_info;
  VkDeviceQueueCreateInfo queue_create_info[NUS_gpu_->queue_family_count];
  unsigned int gpu_queue_count,
    i;

  gpu_queue_count = 0;
  for(i = 0; i < NUS_gpu_->queue_family_count; ++i){
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

  const char *exten[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };
  device_create_info.enabledExtensionCount = 1;
  device_create_info.ppEnabledExtensionNames = exten;
  device_create_info.pEnabledFeatures = NULL;
  if(vkCreateDevice(physical_device, &device_create_info, NULL, &NUS_gpu_->device)
     != VK_SUCCESS){
    printf("ERROR::failed to create logical device\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_get_gpu_queues(VkDevice logical_device, NUS_gpu *NUS_gpu_)
{
  unsigned int i,
    j;
  for(i = 0; i < NUS_gpu_->queue_family_count; ++i){
    for(j = 0; j < NUS_gpu_->queue_families[i].queue_count; ++j){
      vkGetDeviceQueue(logical_device, i, 0,
		       NUS_gpu_->queue_families[i].queues + j);
    }
  }
  return NUS_SUCCESS;
}
