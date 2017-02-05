#include "NUS_queue_family.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

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
void nus_queue_family_free
(NUS_queue_family *NUS_queue_family_, VkDevice logical_device)
{
  if(NUS_queue_family_->priorities){
    free(NUS_queue_family_->priorities);
    NUS_queue_family_->priorities = NULL;
  }
  if(NUS_queue_family_->queues){
    free(NUS_queue_family_->queues);
    NUS_queue_family_->queues = NULL;
  }
  vkDestroyCommandPool(logical_device, NUS_queue_family_->command_pool, NULL);
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
NUS_result nus_queue_family_build_queues
(VkDevice logical_device, NUS_queue_family *NUS_queue_family_)
{
  unsigned int i;

  VkCommandPoolCreateInfo command_pool_create_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueFamilyIndex = NUS_queue_family_->family_index
  };
  if(vkCreateCommandPool(logical_device, &command_pool_create_info,
			 NULL, &NUS_queue_family_->command_pool) != VK_SUCCESS){
    printf("ERROR::failed to create command pool\n");
    return NUS_FAILURE;
  }
  
  NUS_queue_family_->queues = malloc(sizeof(*NUS_queue_family_->queues)
				       * NUS_queue_family_->queue_count);
  
  for(i = 0; i < NUS_queue_family_->queue_count; ++i){
    nus_command_queue_build(logical_device, NUS_queue_family_->family_index, i,
			    NUS_queue_family_->queues + i);
  }
  return NUS_SUCCESS;
}
NUS_result nus_queue_family_test_surface_support
(VkPhysicalDevice physical_device, VkSurfaceKHR surface,
 NUS_queue_family *NUS_queue_family_)
{
  // Tests for surface support after surface has been created
  unsigned int device_support;
  vkGetPhysicalDeviceSurfaceSupportKHR(physical_device,
				       NUS_queue_family_->family_index,
				       surface, &device_support);
  if(VK_TRUE == device_support){
    NUS_queue_family_->flags |= NUS_QUEUE_FAMILY_SUPPORT_PRESENT;
  }
  return NUS_SUCCESS;
}
NUS_result nus_queue_family_find_suitable_queue
(NUS_queue_family NUS_queue_family_, unsigned int *command_queue_index)
{
  unsigned int i,
    least_command_buffer_count = UINT_MAX;
  
  for(i = 0; i < NUS_queue_family_.queue_count; ++i){
    if(NUS_queue_family_.queues[i].command_buffer_count <
       least_command_buffer_count){
      // If queue has the least command buffers queued up of the queues we've checked
      *command_queue_index = i;
      least_command_buffer_count = NUS_queue_family_.queues[i].command_buffer_count;
    }
  }
  return NUS_SUCCESS;
}

NUS_result nus_queue_family_add_command_buffer
(NUS_queue_family NUS_queue_family_, VkDevice logical_device,
 VkCommandBuffer *command_buffer)
{
  unsigned int command_queue_index = UINT_MAX;
  if(nus_queue_family_find_suitable_queue(NUS_queue_family_,
					  &command_queue_index) != NUS_SUCCESS){
    printf("ERROR::failed to find queue family suitable queue\n");
    return NUS_FAILURE;
  }
  if(nus_command_queue_add_buffer(NUS_queue_family_.queues + command_queue_index,
				  logical_device, NUS_queue_family_.command_pool,
				  command_buffer) != NUS_SUCCESS){
    printf("ERROR::failed to add queue family command buffer\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
NUS_result nus_queue_family_submit_commands
(NUS_queue_family NUS_queue_family_, VkDevice logical_device)
{
  unsigned int i;
  for(i = 0; i < NUS_queue_family_.queue_count; ++i){
    if(nus_command_queue_submit_commands(NUS_queue_family_.queues + i,
					 logical_device,
					 NUS_queue_family_.command_pool) !=
       NUS_SUCCESS){
      printf("ERROR::failed to submit command queues\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
