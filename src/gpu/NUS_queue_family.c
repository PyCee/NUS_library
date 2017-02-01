#include "NUS_queue_family.h"
#include <stdlib.h>
#include <stdio.h>

NUS_result nus_queue_family_build
(VkQueueFamilyProperties queue_family_properties, unsigned int queue_family_index,
 NUS_queue_family *NUS_queue_family_)
{
  unsigned char i;
  float default_priority;

  NUS_queue_family_->family_index = queue_family_index;
  if(0 == queue_family_properties.queueCount){
    printf("ERROR::no queues found in queue family\n");
    return NUS_FAILURE;
  }
  NUS_queue_family_->queue_count = queue_family_properties.queueCount;
  
  NUS_queue_family_->queues = malloc(sizeof(*NUS_queue_family_->queues)
				       * NUS_queue_family_->queue_count);
  NUS_queue_family_->priorities =
    malloc(sizeof(*NUS_queue_family_->priorities) *
	   NUS_queue_family_->queue_count);
  
  default_priority = 1.0f / (float)NUS_queue_family_->queue_count;
  for(i = 0; i < NUS_queue_family_->queue_count; ++i){
    NUS_queue_family_->priorities[i] = default_priority;
  }
  NUS_queue_family_->queue_create_info = (VkDeviceQueueCreateInfo){
    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueFamilyIndex = queue_family_index,
    .queueCount = NUS_queue_family_->queue_count,
    .pQueuePriorities = NUS_queue_family_->priorities
  };

  NUS_queue_family_->flags = 0;
  if(queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT){
    NUS_queue_family_->flags |= NUS_QUEUE_FAMILY_SUPPORT_GRAPHICS;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT){
    NUS_queue_family_->flags |= NUS_QUEUE_FAMILY_SUPPORT_COMPUTE;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT){
    NUS_queue_family_->flags |= NUS_QUEUE_FAMILY_SUPPORT_TRANSFER;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT){
    NUS_queue_family_->flags |= NUS_QUEUE_FAMILY_SUPPORT_SPARSE;
  }
  
  return NUS_SUCCESS;
}
void nus_queue_family_free(NUS_queue_family *NUS_queue_family_)
{
  if(NUS_queue_family_->priorities){
    free(NUS_queue_family_->priorities);
    NUS_queue_family_->priorities = NULL;
  }
  if(NUS_queue_family_->queues){
    free(NUS_queue_family_->queues);
    NUS_queue_family_->queues = NULL;
  }
}
void nus_queue_family_print(NUS_queue_family NUS_queue_family_)
{
  printf("queue family has %d queue(s) and supports:\n", NUS_queue_family_.queue_count);
  printf("graphics: %d\n", !!(NUS_queue_family_.flags &
			      NUS_QUEUE_FAMILY_SUPPORT_GRAPHICS));
  printf("compute: %d\n", !!(NUS_queue_family_.flags &
			     NUS_QUEUE_FAMILY_SUPPORT_COMPUTE));
  printf("transfer: %d\n", !!(NUS_queue_family_.flags &
			      NUS_QUEUE_FAMILY_SUPPORT_TRANSFER));
  printf("sparse: %d\n", !!(NUS_queue_family_.flags &
			    NUS_QUEUE_FAMILY_SUPPORT_SPARSE));
  printf("present: %d\n", !!(NUS_queue_family_.flags &
			     NUS_QUEUE_FAMILY_SUPPORT_PRESENT));
}
NUS_result nus_queue_family_get_queues
(VkDevice logical_device, NUS_queue_family *NUS_queue_family_)
{
  unsigned int i;
  for(i = 0; i < NUS_queue_family_->queue_count; ++i){
    vkGetDeviceQueue(logical_device, NUS_queue_family_->family_index, i,
		     NUS_queue_family_->queues + i);
  }
  return NUS_SUCCESS;
}
NUS_result nus_queue_family_test_surface_support
(VkPhysicalDevice physical_device, VkSurfaceKHR surface, NUS_queue_family *NUS_queue_family_)
{
  unsigned int device_support;
  vkGetPhysicalDeviceSurfaceSupportKHR(physical_device,
				       NUS_queue_family_->family_index,
				       surface, &device_support);
  if(VK_TRUE == device_support){
    NUS_queue_family_->flags |= NUS_QUEUE_FAMILY_SUPPORT_PRESENT;
  }
  return NUS_SUCCESS;
}
