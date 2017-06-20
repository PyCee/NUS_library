#include "NUS_queue_family.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "NUS_gpu.h"

NUS_result nus_queue_family_build
(VkQueueFamilyProperties queue_family_properties, unsigned int queue_family_index,
 NUS_queue_family *p_queue_family)
{
  p_queue_family->family_index = queue_family_index;
  if(0 == queue_family_properties.queueCount){
    printf("ERROR::no submission_queues found in queue family\n");
    return NUS_FAILURE;
  }
  p_queue_family->submission_queue_count = queue_family_properties.queueCount;
  
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
  if(p_queue_family->p_submission_queues){
    for(i = 0; i < p_queue_family->submission_queue_count; ++i){
      nus_submission_queue_free(p_queue_family->p_submission_queues + i, logical_device);
    }
    free(p_queue_family->p_submission_queues);
    p_queue_family->p_submission_queues = NULL;
  }
  vkResetCommandPool(logical_device, p_queue_family->command_pool,
		     VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
  vkDestroyCommandPool(logical_device, p_queue_family->command_pool, NULL);
}
void nus_queue_family_print(NUS_queue_family queue_family)
{
  printf("queue family has %d queue(s) and supports:\n",
	 queue_family.submission_queue_count);
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
NUS_result nus_queue_family_build_submission_queues
(VkDevice logical_device, NUS_queue_family *p_queue_family)
{
  unsigned int i;

  VkCommandPoolCreateInfo command_pool_create_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    //.flags = 0,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = p_queue_family->family_index
  };
  if(vkCreateCommandPool(logical_device, &command_pool_create_info,
			 NULL, &p_queue_family->command_pool) != VK_SUCCESS){
    printf("ERROR::failed to create queue family command pool\n");
    return NUS_FAILURE;
  }
  
  p_queue_family->p_submission_queues = malloc(sizeof(*p_queue_family->p_submission_queues)
					     * p_queue_family->submission_queue_count);
  
  for(i = 0; i < p_queue_family->submission_queue_count; ++i){
    nus_submission_queue_build(logical_device, p_queue_family->family_index, i,
			       p_queue_family->p_submission_queues + i);
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
NUS_result nus_queue_family_submit_commands
(NUS_queue_family *p_queue_family, VkDevice logical_device)
{
  unsigned int i;
  
  for(i = 0; i < p_queue_family->submission_queue_count; ++i){
    if(nus_submission_queue_submit(p_queue_family->p_submission_queues + i,
				   logical_device) != NUS_SUCCESS){
      printf("ERROR::failed to submit command submission_queues\n");
      return NUS_FAILURE;
    }
  }
  return NUS_SUCCESS;
}
