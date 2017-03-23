#include "NUS_presentation_surface.h"
#include "../io/NUS_window.h"
#include "../gpu/NUS_multi_gpu.h"
#include "../gpu/NUS_queue_info.h"
#include "../gpu/NUS_vulkan_instance.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static void nus_presentation_surface_build_command_buffers
(NUS_presentation_surface *);

NUS_result nus_presentation_surface_build
(NUS_window NUS_window_, NUS_vulkan_instance NUS_vulkan_instance_,
 NUS_multi_gpu *NUS_multi_gpu_,
 NUS_presentation_surface *p_presentation_surface)
{
#if defined(NUS_OS_WINDOWS)
  VkWin32SurfaceCreateInfoKHR surface_create_info;
  surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  surface_create_info.pNext = NULL;
  surface_create_info.flags = 0;
  //surface_create_info.hinstance = Window.Instance;//TODO support windows
  //surface_create_info.hwind = Window.Handle;
  if(vkCreateWin32SurfaceKHR(NUS_vulkan_instance_.instance, &surface_create_info,
			      NULL, &p_presentation_surface.surface) != VK_SUCCESS){
    printf("ERROR::unable to create Win32 vulkan surface\n");
    return NUS_FAILURE;
  }
#elif defined(NUS_OS_UNIX)
  VkXcbSurfaceCreateInfoKHR surface_create_info;
  surface_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
  surface_create_info.pNext = NULL;
  surface_create_info.flags = 0;
  surface_create_info.connection = NUS_window_.connection;
  surface_create_info.window = NUS_window_.window;
  if(vkCreateXcbSurfaceKHR(NUS_vulkan_instance_.instance, &surface_create_info,
			   NULL, &p_presentation_surface->surface) != VK_SUCCESS){
    printf("ERROR::unable to create XCB vulkan surface\n");
    return NUS_FAILURE;
  }
#endif
  nus_multi_gpu_check_surface_support(p_presentation_surface->surface,
				      NUS_multi_gpu_);

  nus_multi_gpu_find_queue_info(*NUS_multi_gpu_,
				    NUS_QUEUE_FAMILY_SUPPORT_PRESENT,
				    &p_presentation_surface->queue_info);
  
  nus_bind_device_vulkan_library(p_presentation_surface->queue_info.p_gpu->functions);

  if(nus_swapchain_build(*p_presentation_surface->queue_info.p_gpu,
			 p_presentation_surface->surface,
			 &p_presentation_surface->swapchain) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build presentation surface swapchain\n");
    return NUS_FAILURE;
  }

  if(nus_texture_build(*p_presentation_surface->queue_info.p_gpu,
		       p_presentation_surface->swapchain.extent.width,
		       p_presentation_surface->swapchain.extent.height,
		       p_presentation_surface->swapchain.format.format,
		       VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
		       VK_IMAGE_USAGE_TRANSFER_DST_BIT |
		       VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		       &p_presentation_surface->render_target) != NUS_SUCCESS){
    printf("ERROR::failed to build render_target texture\n");
    return NUS_FAILURE;
  }
  
  // Create semaphores vital to managing render_target access
  VkSemaphoreCreateInfo semaphore_create_info = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0
  };
  if(vkCreateSemaphore(p_presentation_surface->queue_info.p_gpu->logical_device,
		       &semaphore_create_info, NULL,
		       &p_presentation_surface->render_copied) !=
     VK_SUCCESS){
    printf("ERROR::failed to create surface semaphore: image_available\n");
    return NUS_FAILURE;
  }
  if(vkCreateSemaphore(p_presentation_surface->queue_info.p_gpu->logical_device,
		       &semaphore_create_info, NULL,
		       &p_presentation_surface->image_available) !=
     VK_SUCCESS){
    printf("ERROR::failed to create surface semaphore: image_available\n");
    return NUS_FAILURE;
  }
  if(vkCreateSemaphore(p_presentation_surface->queue_info.p_gpu->logical_device,
		       &semaphore_create_info, NULL,
		       &p_presentation_surface->image_presentable) !=
     VK_SUCCESS){
    printf("ERROR::failed to create surface semaphore: image_presentable\n");
    return NUS_FAILURE;
  }
  if(nus_swapchain_new_image(*p_presentation_surface->queue_info.p_gpu,
			     p_presentation_surface->surface,
			     p_presentation_surface->image_available,
			     &p_presentation_surface->swapchain) != NUS_SUCCESS){
    printf("ERROR::failed to get new swapchain image upon build\n");
    return NUS_FAILURE;
  }
  nus_presentation_surface_build_command_buffers(p_presentation_surface);
  return NUS_SUCCESS;
}
void nus_presentation_surface_free
(NUS_vulkan_instance NUS_vulkan_instance_,
 NUS_presentation_surface *p_presentation_surface)
{
  nus_swapchain_free(*p_presentation_surface->queue_info.p_gpu,
		     &p_presentation_surface->swapchain);
  // Free semaphores
  if(p_presentation_surface->render_copied != VK_NULL_HANDLE){
    vkDestroySemaphore(p_presentation_surface->queue_info.p_gpu->logical_device,
		       p_presentation_surface->render_copied, NULL);
    p_presentation_surface->render_copied = VK_NULL_HANDLE;
  }
  if(p_presentation_surface->image_presentable != VK_NULL_HANDLE){
    vkDestroySemaphore(p_presentation_surface->queue_info.p_gpu->logical_device,
		       p_presentation_surface->image_presentable, NULL);
    p_presentation_surface->image_presentable = VK_NULL_HANDLE;
  }
  if(p_presentation_surface->image_available != VK_NULL_HANDLE){
    vkDestroySemaphore(p_presentation_surface->queue_info.p_gpu->logical_device,
		       p_presentation_surface->image_available, NULL);
    p_presentation_surface->image_available = VK_NULL_HANDLE;
  }
  nus_texture_free(*p_presentation_surface->queue_info.p_gpu,
		   &p_presentation_surface->render_target);
  
  vkDestroySurfaceKHR(NUS_vulkan_instance_.instance,
		      p_presentation_surface->surface, NULL);
}
NUS_result nus_presentation_surface_present
(NUS_presentation_surface *p_presentation_surface)
{
  nus_command_group_add_semaphores(p_presentation_surface->queue_info.p_command_group,
				   1, &p_presentation_surface->image_presentable,
				   1, &p_presentation_surface->render_copied);
  nus_command_group_append(p_presentation_surface->queue_info.p_command_group,
			   p_presentation_surface->render_target_copy_command_buffers
			   [p_presentation_surface->swapchain.image_index]);
  nus_queue_info_submit(p_presentation_surface->queue_info);
  
  if(nus_swapchain_present(p_presentation_surface->queue_info.p_gpu,
			   p_presentation_surface->surface,
			   p_presentation_surface->render_copied,
			   &p_presentation_surface->swapchain) != NUS_SUCCESS){
    printf("ERROR::failed to present to swapchain\n");
    return NUS_FAILURE;
  }
  if(nus_swapchain_new_image(*p_presentation_surface->queue_info.p_gpu,
			     p_presentation_surface->surface,
			     p_presentation_surface->image_available,
			     &p_presentation_surface->swapchain) != NUS_SUCCESS){
    printf("ERROR::failed to get new swapchain image\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}

static void nus_presentation_surface_build_command_buffers
(NUS_presentation_surface *p_presentation_surface)
{
  unsigned int i;
  // Record command_buffers for copying the render target to the swapchain images
  
  VkCommandBufferBeginInfo command_buffer_begin_info = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    NULL,
    VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
    NULL
  };
  VkImageSubresourceRange image_subresource_range = {
    VK_IMAGE_ASPECT_COLOR_BIT,
    0,
    1,
    0,
    1
  };
  VkImageSubresourceLayers sub_resource = {
    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    .baseArrayLayer = 0,
    .mipLevel = 0,
    .layerCount = 1
  };
  VkImageCopy region = {
    .srcSubresource = sub_resource,
    .dstSubresource = sub_resource,
    .srcOffset = {0, 0, 0},
    .dstOffset = {0, 0, 0},
    .extent.width = p_presentation_surface->swapchain.extent.width,
    .extent.height = p_presentation_surface->swapchain.extent.height,
    .extent.depth = 1
  };
  p_presentation_surface->render_target_copy_command_buffers =
    malloc(sizeof(*p_presentation_surface->render_target_copy_command_buffers) *
	   p_presentation_surface->swapchain.image_count);
  for(i = 0; i < p_presentation_surface->swapchain.image_count; ++i){
    nus_queue_info_add_buffer(p_presentation_surface->queue_info,
				  p_presentation_surface->
				  render_target_copy_command_buffers + i);
    VkImageMemoryBarrier setup_barriers[] = {
      {
	.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
	.pNext = NULL,
	.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
	.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
	.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
	.srcQueueFamilyIndex = p_presentation_surface->queue_info.queue_family_index,
	.dstQueueFamilyIndex = p_presentation_surface->queue_info.queue_family_index,
	.image = p_presentation_surface->render_target.image,
	.subresourceRange = image_subresource_range
      },
      {
	.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
	.pNext = NULL,
	.srcAccessMask = 0,
	.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
	.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	.srcQueueFamilyIndex = p_presentation_surface->queue_info.queue_family_index,
	.dstQueueFamilyIndex = p_presentation_surface->queue_info.queue_family_index,
	.image = p_presentation_surface->swapchain.images[i],
	.subresourceRange = image_subresource_range
      }
    };
    VkImageMemoryBarrier teardown_barriers[] = {
      {
	.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
	.pNext = NULL,
	.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
	.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
	.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
	.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	.srcQueueFamilyIndex = p_presentation_surface->queue_info.queue_family_index,
	.dstQueueFamilyIndex = p_presentation_surface->queue_info.queue_family_index,
	.image = p_presentation_surface->render_target.image,
	.subresourceRange = image_subresource_range
      },
      {
	.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
	.pNext = NULL,
	.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
	.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
	.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	.srcQueueFamilyIndex = p_presentation_surface->queue_info.queue_family_index,
	.dstQueueFamilyIndex = p_presentation_surface->queue_info.queue_family_index,
	.image = p_presentation_surface->swapchain.images[i],
	.subresourceRange = image_subresource_range
      }
    };
    
    vkBeginCommandBuffer(p_presentation_surface->render_target_copy_command_buffers[i],
			 &command_buffer_begin_info);
    vkCmdPipelineBarrier(p_presentation_surface->render_target_copy_command_buffers[i],
			 VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			 VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			 0, 0, NULL, 0, NULL, 2, setup_barriers);
    vkCmdCopyImage(p_presentation_surface->render_target_copy_command_buffers[i],
		   p_presentation_surface->render_target.image,
		   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		   p_presentation_surface->swapchain.images[i],
		   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		   1, &region);
    vkCmdPipelineBarrier(p_presentation_surface->render_target_copy_command_buffers[i],
			 VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			 VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			 0, 0, NULL, 0, NULL, 2, teardown_barriers);
    vkEndCommandBuffer(p_presentation_surface->render_target_copy_command_buffers[i]);
  }
}
