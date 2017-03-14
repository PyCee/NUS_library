#include "NUS_queue_family.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

NUS_result nus_queue_family_build
(VkQueueFamilyProperties queue_family_properties, unsigned int queue_family_index,
 NUS_queue_family *NUS_queue_family_)
{
  NUS_queue_family_->family_index = queue_family_index;
  if(0 == queue_family_properties.queueCount){
    printf("ERROR::no queues found in queue family\n");
    return NUS_FAILURE;
  }
  NUS_queue_family_->queue_count = queue_family_properties.queueCount;
  
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
  unsigned int i,
    j;
  if(NUS_queue_family_->queues){
    for(i = 0; i < NUS_queue_family_->queue_count; ++i){
      nus_command_queue_free(NUS_queue_family_->queues + i);
    }
    free(NUS_queue_family_->queues);
    NUS_queue_family_->queues = NULL;
  }
  
  for(i = 0; i < NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT; ++i){
    vkDestroyCommandPool(logical_device, NUS_queue_family_->command_pool[i], NULL);
  }
  for(i = 0; i < NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT; ++i){
    for(j = 0; j < NUS_queue_family_->queue_count; ++j){
      vkDestroyFence(logical_device, NUS_queue_family_->buffer_fences[i][j], NULL);
    }
    free(NUS_queue_family_->buffer_fences[i]);
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
NUS_result nus_queue_family_build_queues
(VkDevice logical_device, NUS_queue_family *NUS_queue_family_)
{
  unsigned int i,
    j;

  VkCommandPoolCreateInfo command_pool_create_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = NUS_queue_family_->family_index
  };
  for(i = 0; i < NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT; ++i){
    if(vkCreateCommandPool(logical_device, &command_pool_create_info,
			   NULL, &NUS_queue_family_->command_pool[i]) != VK_SUCCESS){
    printf("ERROR::failed to create queue family command pool\n");
    return NUS_FAILURE;
    }
  }
  
  VkFenceCreateInfo fence_create_info = {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .pNext = 0,
    .flags = 0
  };
  for(i = 0; i < NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT; ++i){
    NUS_queue_family_->buffer_fences[i] =
      malloc(sizeof(*NUS_queue_family_->buffer_fences[i]) *
	     NUS_queue_family_->queue_count);
    for(j = 0; j < NUS_queue_family_->queue_count; ++j){
      if(vkCreateFence(logical_device, &fence_create_info, NULL,
		       &NUS_queue_family_->buffer_fences[i][j]) != VK_SUCCESS){
	printf("ERROR::failed to create queue family fence\n");
	return NUS_FAILURE;
      }
    }
  }
  NUS_queue_family_->active_command_pool_index = 0;
  
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
				  logical_device,
				  NUS_queue_family_.command_pool
				  [NUS_queue_family_.active_command_pool_index],
				  command_buffer) != NUS_SUCCESS){
    printf("ERROR::failed to add queue family command buffer\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
NUS_result nus_queue_family_submit_commands
(NUS_queue_family *NUS_queue_family_, VkDevice logical_device)
{
  unsigned int i;
  
  static unsigned int call_count = 0;
  
  for(i = 0; i < NUS_queue_family_->queue_count; ++i){
    if(nus_command_queue_submit(NUS_queue_family_->queues + i,
				logical_device,
				NUS_queue_family_->buffer_fences
				[NUS_queue_family_->active_command_pool_index][i])
       != NUS_SUCCESS){
      printf("ERROR::failed to submit command queues\n");
      return NUS_FAILURE;
    }
  }
  
  // Increment active command pool index
  NUS_queue_family_->active_command_pool_index += 1;
  if(NUS_queue_family_->active_command_pool_index >=
     NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT){
    NUS_queue_family_->active_command_pool_index -=
      NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT;
  }
  if(call_count >= NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT - 1){
    // If all command pools to have been used
    // Reset next active command pool and fences
    
    // Wait for up to 1 millisecond for comand buffers to finish
    vkWaitForFences(logical_device, NUS_queue_family_->queue_count,
		    NUS_queue_family_->buffer_fences
		    [NUS_queue_family_->active_command_pool_index],
		    VK_TRUE, 1000000);
    vkResetFences(logical_device, NUS_queue_family_->queue_count,
		  NUS_queue_family_->buffer_fences
		  [NUS_queue_family_->active_command_pool_index]);
    vkResetCommandPool(logical_device,
		       NUS_queue_family_->command_pool
		       [NUS_queue_family_->active_command_pool_index],
		       VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
  } else{
    ++call_count;
  }
  return NUS_SUCCESS;
}
