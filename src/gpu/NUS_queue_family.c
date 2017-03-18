#include "NUS_queue_family.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "NUS_gpu.h"
#include "NUS_suitable_queue.h"

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
  unsigned int i;
  if(p_queue_family->command_groups){
    for(i = 0; i < p_queue_family->command_group_count; ++i){
      nus_command_group_free(p_queue_family->command_groups + i);
    }
    free(p_queue_family->command_groups);
    p_queue_family->command_groups = NULL;
  }
  vkResetCommandPool(logical_device, p_queue_family->command_pool,
		     VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
  vkDestroyCommandPool(logical_device, p_queue_family->command_pool, NULL);
}
void nus_queue_family_print(NUS_queue_family queue_family)
{
  printf("queue family has %d queue(s) and supports:\n",
	 queue_family.command_group_count);
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
  unsigned int i;

  VkCommandPoolCreateInfo command_pool_create_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueFamilyIndex = p_queue_family->family_index
  };
  if(vkCreateCommandPool(logical_device, &command_pool_create_info,
			 NULL, &p_queue_family->command_pool) != VK_SUCCESS){
    printf("ERROR::failed to create queue family command pool\n");
    return NUS_FAILURE;
  }
  
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
(NUS_queue_family queue_family, NUS_suitable_queue *info)
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
NUS_result nus_queue_family_allocate_command_buffer
(NUS_queue_family queue_family, VkDevice logical_device,
 VkCommandBuffer *p_command_buffer)
{
  VkCommandBufferAllocateInfo command_buffer_allocate_info = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    NULL,
    queue_family.command_pool,
    VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    1
  };
  if(vkAllocateCommandBuffers(logical_device, &command_buffer_allocate_info,
			      p_command_buffer) != VK_SUCCESS){
    printf("ERROR::failed to allocate command buffers\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
NUS_result nus_queue_family_submit_commands
(NUS_queue_family *p_queue_family, VkDevice logical_device)
{
  unsigned int i;
  
  for(i = 0; i < p_queue_family->command_group_count; ++i){
    if(nus_command_group_submit(p_queue_family->command_groups + i,
				logical_device,
			        VK_NULL_HANDLE) != NUS_SUCCESS){
      printf("ERROR::failed to submit command command_groups\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
