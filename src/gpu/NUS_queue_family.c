#include "NUS_queue_family.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "NUS_gpu.h"
#include "NUS_suitable_queue_info.h"

NUS_result nus_queue_family_build
(VkQueueFamilyProperties queue_family_properties, unsigned int queue_family_index,
 NUS_queue_family *p_queue_family)
{
  p_queue_family->family_index = queue_family_index;
  if(0 == queue_family_properties.queueCount){
    printf("ERROR::no command_groups found in queue family\n");
    return NUS_FAILURE;
  }
  p_queue_family->command_group_count = queue_family_properties.queueCount;
  
  p_queue_family->flags = 0;
  if(queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT){
    p_queue_family->flags |= NUS_QUEUE_FAMILY_SUPPORT_GRAPHICS;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT){
    p_queue_family->flags |= NUS_QUEUE_FAMILY_SUPPORT_COMPUTE;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT){
    p_queue_family->flags |= NUS_QUEUE_FAMILY_SUPPORT_TRANSFER;
  }
  if(queue_family_properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT){
    p_queue_family->flags |= NUS_QUEUE_FAMILY_SUPPORT_SPARSE;
  }
  return NUS_SUCCESS;
}
void nus_queue_family_free
(NUS_queue_family *p_queue_family, VkDevice logical_device)
{
  unsigned int i,
    j;
  if(p_queue_family->command_groups){
    for(i = 0; i < p_queue_family->command_group_count; ++i){
      nus_command_group_free(p_queue_family->command_groups + i);
    }
    free(p_queue_family->command_groups);
    p_queue_family->command_groups = NULL;
  }
  
  for(i = 0; i < NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT; ++i){
    vkDestroyCommandPool(logical_device, p_queue_family->command_pool[i], NULL);
  }
  for(i = 0; i < NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT; ++i){
    for(j = 0; j < p_queue_family->command_group_count; ++j){
      vkDestroyFence(logical_device, p_queue_family->buffer_fences[i][j], NULL);
    }
    free(p_queue_family->buffer_fences[i]);
  }
}
void nus_queue_family_print(NUS_queue_family queue_family)
{
  printf("queue family has %d queue(s) and supports:\n", queue_family.command_group_count);
  printf("graphics: %d\n", !!(queue_family.flags &
			      NUS_QUEUE_FAMILY_SUPPORT_GRAPHICS));
  printf("compute: %d\n", !!(queue_family.flags &
			     NUS_QUEUE_FAMILY_SUPPORT_COMPUTE));
  printf("transfer: %d\n", !!(queue_family.flags &
			      NUS_QUEUE_FAMILY_SUPPORT_TRANSFER));
  printf("sparse: %d\n", !!(queue_family.flags &
			    NUS_QUEUE_FAMILY_SUPPORT_SPARSE));
  printf("present: %d\n", !!(queue_family.flags &
			     NUS_QUEUE_FAMILY_SUPPORT_PRESENT));
}
NUS_result nus_queue_family_build_command_groups
(VkDevice logical_device, NUS_queue_family *p_queue_family)
{
  unsigned int i,
    j;

  VkCommandPoolCreateInfo command_pool_create_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = p_queue_family->family_index
  };
  for(i = 0; i < NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT; ++i){
    if(vkCreateCommandPool(logical_device, &command_pool_create_info,
			   NULL, &p_queue_family->command_pool[i]) != VK_SUCCESS){
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
    p_queue_family->buffer_fences[i] =
      malloc(sizeof(*p_queue_family->buffer_fences[i]) *
	     p_queue_family->command_group_count);
    for(j = 0; j < p_queue_family->command_group_count; ++j){
      if(vkCreateFence(logical_device, &fence_create_info, NULL,
		       &p_queue_family->buffer_fences[i][j]) != VK_SUCCESS){
	printf("ERROR::failed to create queue family fence\n");
	return NUS_FAILURE;
      }
    }
  }
  p_queue_family->active_command_pool_index = 0;
  
  p_queue_family->command_groups = malloc(sizeof(*p_queue_family->command_groups)
				       * p_queue_family->command_group_count);
  
  for(i = 0; i < p_queue_family->command_group_count; ++i){
    nus_command_group_build(logical_device, p_queue_family->family_index, i,
			    p_queue_family->command_groups + i);
  }
  return NUS_SUCCESS;
}
NUS_result nus_queue_family_test_surface_support
(VkPhysicalDevice physical_device, VkSurfaceKHR surface,
 NUS_queue_family *p_queue_family)
{
  // Tests for surface support after surface has been created
  unsigned int device_support;
  vkGetPhysicalDeviceSurfaceSupportKHR(physical_device,
				       p_queue_family->family_index,
				       surface, &device_support);
  if(VK_TRUE == device_support){
    p_queue_family->flags |= NUS_QUEUE_FAMILY_SUPPORT_PRESENT;
  }
  return NUS_SUCCESS;
}
NUS_result nus_queue_family_find_suitable_queue
(NUS_queue_family queue_family, NUS_suitable_queue_info *info)
{
  unsigned int i,
    least_workload = UINT_MAX;
  
  for(i = 0; i < queue_family.command_group_count; ++i){
    if(queue_family.command_groups[i].command_buffer_count <
       least_workload){
      // If command group has the least commands of the command_groups we've checked
      info->command_group_index = i;
      info->p_command_group = queue_family.command_groups + i;
      least_workload = queue_family.command_groups[i].command_buffer_count;
    }
  }
  return NUS_SUCCESS;
}
NUS_result nus_queue_family_submit_commands
(NUS_queue_family *p_queue_family, VkDevice logical_device)
{
  unsigned int i;
  
  static unsigned int call_count = 0;
  
  for(i = 0; i < p_queue_family->command_group_count; ++i){
    if(nus_command_group_submit(p_queue_family->command_groups + i,
				logical_device,
				p_queue_family->buffer_fences
				[p_queue_family->active_command_pool_index][i])
       != NUS_SUCCESS){
      printf("ERROR::failed to submit command command_groups\n");
      return NUS_FAILURE;
    }
  }
  
  // Increment active command pool index
  p_queue_family->active_command_pool_index += 1;
  if(p_queue_family->active_command_pool_index >=
     NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT){
    p_queue_family->active_command_pool_index -=
      NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT;
  }
  if(call_count >= NUS_QUEUE_FAMILY_COMMAND_POOL_COUNT - 1){
    // If all command pools to have been used
    // Reset next active command pool and fences
    
    // Wait for up to 1 millisecond for comand buffers to finish
    vkWaitForFences(logical_device, p_queue_family->command_group_count,
		    p_queue_family->buffer_fences
		    [p_queue_family->active_command_pool_index],
		    VK_TRUE, 1000000);
    vkResetFences(logical_device, p_queue_family->command_group_count,
		  p_queue_family->buffer_fences
		  [p_queue_family->active_command_pool_index]);
    vkResetCommandPool(logical_device,
		       p_queue_family->command_pool
		       [p_queue_family->active_command_pool_index],
		       VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
  } else{
    ++call_count;
  }
  return NUS_SUCCESS;
}
