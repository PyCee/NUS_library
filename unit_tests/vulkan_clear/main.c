
#include <stdio.h>
#include <string.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#define NUS_SYSTEM_RENDER
#include <NUS/NUS_library.h>
#include <limits.h>

#define PROGRAM_NAME "unit_test-vulkan_clear"

void close_win(void);
char run;
void close_win(void);

int main(int argc, char *argv[])
{
  unsigned int i;
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  nus_load_global_vulkan_library();

  
  NUS_window win;
  if(nus_window_build(PROGRAM_NAME, 600, 400, &win) != NUS_SUCCESS){
    printf("ERROR::failed to create window\n");
    return -1;
  }
  NUS_event_handler eve;
  if(nus_event_handler_build(&eve) != NUS_SUCCESS){
    printf("ERROR::failed to build event handler\n");
    return -1;
  }
  eve.close_window = close_win;
  nus_event_handler_set(&eve);
  
  
  NUS_vulkan_instance vulkan_instance;
  nus_vulkan_instance_init(&vulkan_instance);
  nus_vulkan_instance_add_extension(VK_KHR_SURFACE_EXTENSION_NAME,
				    &vulkan_instance);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  nus_vulkan_instance_add_extension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
				    &vulkan_instance);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  nus_vulkan_instance_add_extension(VK_KHR_XCB_SURFACE_EXTENSION_NAME,
				    &vulkan_instance);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
  nus_vulkan_instance_add_extension(VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
				    &vulkan_instance);
#endif
  
  if(nus_vulkan_instance_build(&vulkan_instance)
     != NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }
  
  NUS_multi_gpu gpu_g;
  if(nus_multi_gpu_build(vulkan_instance.instance, &gpu_g) != NUS_SUCCESS){
    printf("ERROR::build multi gpu returned NUS_FAILURE\n");
    return -1;
  }
  
  NUS_presentation_surface present;
  if(nus_presentation_surface_build(win, vulkan_instance, &gpu_g, &present)
     != NUS_SUCCESS){
    printf("ERROR::failed to build presentaion surface\n");
    return -1;
  }
  
  nus_multi_gpu_check_surface_support(present.surface, &gpu_g);






  
  //tmp code
  
  nus_bind_device_vulkan_library(gpu_g.gpus[0].functions);
  
  VkSemaphore image_available,
    render_finished;
  VkSemaphoreCreateInfo semaphore_create_info;
  semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphore_create_info.pNext = NULL;
  semaphore_create_info.flags = 0;

  if((vkCreateSemaphore(gpu_g.gpus[0].logical_device, &semaphore_create_info, NULL,
			&image_available) != VK_SUCCESS) ||
     (vkCreateSemaphore(gpu_g.gpus[0].logical_device, &semaphore_create_info, NULL,
		       &render_finished) != VK_SUCCESS)){
    printf("ERROR::failed to create semaphores\n");
    return -1;
  }
  
  VkSurfaceCapabilitiesKHR surface_capabilities;
  if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu_g.physical_devices[0],
					    present.surface, &surface_capabilities)
     != VK_SUCCESS){
    printf("ERROR::failed during surface capability enumeration\n");
    return -1;
  }

  unsigned int surface_format_count;
  if((vkGetPhysicalDeviceSurfaceFormatsKHR(gpu_g.physical_devices[0], present.surface,
					   &surface_format_count, NULL) != VK_SUCCESS)
     || (surface_format_count == 0)){
    printf("ERROR::failed during surface format enumeration\n");
    return -1;
  }
  VkSurfaceFormatKHR surface_formats[surface_format_count];
  if(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu_g.physical_devices[0], present.surface,
					  &surface_format_count,
					  surface_formats) != VK_SUCCESS){
    printf("ERROR::failed during surface format enumeration\n");
    return -1;
  }

  
  unsigned int surface_present_mode_count;
  if((vkGetPhysicalDeviceSurfacePresentModesKHR(gpu_g.physical_devices[0],
						present.surface,
						&surface_present_mode_count,
						NULL) != VK_SUCCESS)
     || (surface_present_mode_count == 0)){
    printf("ERROR::failed during surface format enumeration\n");
    return -1;
  }
  VkPresentModeKHR surface_present_modes[surface_present_mode_count];
  if(vkGetPhysicalDeviceSurfacePresentModesKHR(gpu_g.physical_devices[0],
					       present.surface,
					       &surface_present_mode_count,
					       surface_present_modes) != VK_SUCCESS){
    printf("ERROR::failed during surface format enumeration\n");
    return -1;
  }

  //TODO better code to be sure variable in within range
  unsigned int surface_desired_image_count = surface_capabilities.minImageCount + 1;
  printf("min image count: %d\nmax image count: %d\n",
	 surface_capabilities.minImageCount,
	 surface_capabilities.maxImageCount);
  if(0 == surface_capabilities.maxImageCount){
    printf("\tmax image count of 0, no upper limit to the number of swapchain images\n");
  }

  VkSurfaceFormatKHR format;
  /* If the surface has no preferred format */
  if(1 == surface_format_count &&
     surface_formats[0].format == VK_FORMAT_UNDEFINED){
    format = (VkSurfaceFormatKHR)
      {VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR};
  } else{
    /* Search available formats for our preferred */
    for(i = 0; i < surface_format_count; ++i){
      /* If the current format is our preferred */
      if(VK_FORMAT_R8G8B8A8_UNORM == surface_formats[i].format &&
	 VK_COLORSPACE_SRGB_NONLINEAR_KHR == surface_formats[i].colorSpace){
	format = surface_formats[i];
	break;
      } else if(i == (surface_format_count - 1)){
	/* Else if we are on the last available format */
	/* Set to first available format */
	format = surface_formats[0];
	break;
      }
    }
  }

  /* Obtain desired size of swapchain images */
  VkExtent2D swapchain_extent = {600, 400};//{width, height};
  if(-1 != surface_capabilities.currentExtent.width){
    swapchain_extent = surface_capabilities.currentExtent;
  } else{
    unsigned int w = swapchain_extent.width,
      h = swapchain_extent.height;
    if(w > surface_capabilities.maxImageExtent.width){
      w = surface_capabilities.maxImageExtent.width;
    } else if(w < surface_capabilities.minImageExtent.width){
      w = surface_capabilities.minImageExtent.width;
    }
    if(h > surface_capabilities.maxImageExtent.height){
      h = surface_capabilities.maxImageExtent.height;
    } else if(h < surface_capabilities.minImageExtent.height){
      h = surface_capabilities.minImageExtent.height;
    }
    swapchain_extent = (VkExtent2D){w, h};
  }

  printf("surface usage flags:\n");
  printf("VK_IMAGE_USAGE_TRANSFER_SRC: %d\n",
	 !!(surface_capabilities.supportedUsageFlags &
	    VK_IMAGE_USAGE_TRANSFER_SRC_BIT));
  printf("VK_IMAGE_USAGE_TRANSFER_DST: %d\n",
	 !!(surface_capabilities.supportedUsageFlags &
	    VK_IMAGE_USAGE_TRANSFER_DST_BIT));
  printf("VK_IMAGE_USAGE_SAMPLED: %d\n",
	 !!(surface_capabilities.supportedUsageFlags &
	    VK_IMAGE_USAGE_SAMPLED_BIT));
  printf("VK_IMAGE_USAGE_STORAGE: %d\n",
	 !!(surface_capabilities.supportedUsageFlags &
	    VK_IMAGE_USAGE_STORAGE_BIT));
  printf("VK_IMAGE_USAGE_COLOR_ATTACHMENT: %d\n",
	 !!(surface_capabilities.supportedUsageFlags &
	    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
  printf("VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT: %d\n",
	 !!(surface_capabilities.supportedUsageFlags &
	    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT));
  printf("VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT: %d\n",
	 !!(surface_capabilities.supportedUsageFlags &
	    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT));
  printf("VK_IMAGE_USAGE_INPUT_ATTACHMENT: %d\n",
	 !!(surface_capabilities.supportedUsageFlags &
	    VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT));


  VkSurfaceTransformFlagBitsKHR  surface_transform_bits;
  if(surface_capabilities.supportedTransforms &
     VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR){
    surface_transform_bits = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  }else{
    surface_transform_bits = surface_capabilities.currentTransform;
  }
  
  VkPresentModeKHR present_mode;
  for(i = 0; i < surface_present_mode_count; ++i){
    //request mailbox mode
    if(surface_present_modes[i] == VK_PRESENT_MODE_FIFO_KHR){
      present_mode = surface_present_modes[i];
      break;
    }
    //may check for other present modes
  }

  VkSwapchainKHR old_swapchain = VK_NULL_HANDLE;
  
  VkSwapchainCreateInfoKHR swapchain_create_info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = present.surface,
    .minImageCount = surface_desired_image_count,
    .imageFormat = format.format,
    .imageColorSpace = format.colorSpace,
    .imageExtent = swapchain_extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .preTransform = surface_transform_bits,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = present_mode,
    .clipped = VK_TRUE,
    .oldSwapchain = old_swapchain
  };

  VkSwapchainKHR swapchain;
  if(vkCreateSwapchainKHR(gpu_g.gpus[0].logical_device, &swapchain_create_info,
			   NULL, &swapchain) != VK_SUCCESS){
    printf("ERROR::failed to create swapchain\n");
    return -1;
  }

    
  unsigned int image_index;
  VkResult acquire_result = vkAcquireNextImageKHR(gpu_g.gpus[0].logical_device,
						  swapchain,
						  (unsigned int)-1,
						  image_available,
						  VK_NULL_HANDLE,
						  &image_index);
  switch(acquire_result){
  case VK_SUCCESS:
  case VK_SUBOPTIMAL_KHR:
    break;
  case VK_ERROR_OUT_OF_DATE_KHR:
    //recreate swapchain and command buffers, window has been resized
    printf("khr out of date\n");
    break;
  default:
    printf("ERROR::failed to acquire image\n");
    return -1;
    break;
  }

  //creating command buffers:
  VkCommandPoolCreateInfo command_pool_create_info = {
    VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    NULL,
    0,
    0 // queue family index
  };
  VkCommandPool command_pool;
  if(vkCreateCommandPool(gpu_g.gpus[0].logical_device, &command_pool_create_info,
			 NULL, &command_pool) != VK_SUCCESS){
    printf("ERROR::failed to create command pool\n");
    return -1;
  }
  
  unsigned int image_count;
  if((vkGetSwapchainImagesKHR(gpu_g.gpus[0].logical_device, swapchain,
			     &image_count, NULL ) != VK_SUCCESS) ||
     (image_count == 0)){
    printf("ERROR::failed to obtain count of swapchain images\n");
    return -1;
  }
  VkCommandBuffer command_buffers[image_count];
  VkCommandBufferAllocateInfo command_buffer_allocate_info = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    NULL,
    command_pool,
    VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    image_count
  };
  if(vkAllocateCommandBuffers(gpu_g.gpus[0].logical_device,
			      &command_buffer_allocate_info,
			      command_buffers) != VK_SUCCESS){
    printf("ERROR::failed to allocate command buffers\n");
    return -1;
  }

  //record command buffer
  VkImage swapchain_images[image_count];
  if(vkGetSwapchainImagesKHR(gpu_g.gpus[0].logical_device, swapchain,
			     &image_count, swapchain_images) != VK_SUCCESS){
    printf("ERROR::failed to get swapchain images\n");
    return -1;
  }

  VkCommandBufferBeginInfo command_buffer_begin_info = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    NULL,
    VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
    NULL
  };
  VkClearColorValue clear_color = {
    { 0.0, 0.0, 0.0, 0.0 }
  };

  VkImageSubresourceRange image_subresource_range = {
    VK_IMAGE_ASPECT_COLOR_BIT,
    0,
    1,
    0,
    1
  };
  
  for(i = 0; i < image_count; ++i) {
    VkImageMemoryBarrier barrier_from_present_to_clear = {
      VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
      NULL,                                    // const void                            *pNext
      VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          srcAccessMask
      VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          dstAccessMask
      VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                          oldLayout
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          newLayout
      0,             // uint32_t                               srcQueueFamilyIndex
      0,             // uint32_t                               dstQueueFamilyIndex
      swapchain_images[i],                       // VkImage                                image
      image_subresource_range                     // VkImageSubresourceRange                subresourceRange
    };
    VkImageMemoryBarrier barrier_from_clear_to_present = {
      VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType sType
      NULL,                                    // const void *pNext
      VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags srcAccessMask
      VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags dstAccessMask
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout oldLayout
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout newLayout
      0,             // uint32_t                               srcQueueFamilyIndex
      0,             // uint32_t                               dstQueueFamilyIndex
      swapchain_images[i],                       // VkImage image
      image_subresource_range                     // VkImageSubresourceRange                subresourceRange
    };
    vkBeginCommandBuffer( command_buffers[i], &command_buffer_begin_info );
    vkCmdPipelineBarrier( command_buffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier_from_present_to_clear );
    
    vkCmdClearColorImage( command_buffers[i], swapchain_images[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear_color, 1, &image_subresource_range );
    
    vkCmdPipelineBarrier( command_buffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, NULL, 0, NULL, 1, &barrier_from_clear_to_present );
    if( vkEndCommandBuffer( command_buffers[i] ) != VK_SUCCESS ) {
      printf("ERROR::Could not record command buffers!\n");
      return -1;
    }
  }
  

  //submit queue
  VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
  VkSubmitInfo submit_info = {
    VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
    NULL,                                      // const void                  *pNext
    1,                                            // uint32_t                     waitSemaphoreCount
    &image_available,              // const VkSemaphore           *pWaitSemaphores
    &wait_dst_stage_mask,                         // const VkPipelineStageFlags  *pWaitDstStageMask;
    1,                                            // uint32_t                     commandBufferCount
    &command_buffers[image_index],  // const VkCommandBuffer       *pCommandBuffers
    1,                                            // uint32_t                     signalSemaphoreCount
    &render_finished            // const VkSemaphore           *pSignalSemaphores
  };
  if( vkQueueSubmit( gpu_g.gpus[0].queue_families[0].queues[0].queue, 1,
		     &submit_info, VK_NULL_HANDLE ) != VK_SUCCESS ) {
    printf("ERROR::failed to submit queue\n");
    return -1;
  }
  
  VkPresentInfoKHR present_info = {
    VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           // VkStructureType              sType
    NULL,                                      // const void                  *pNext
    1,                                            // uint32_t                     waitSemaphoreCount
    &render_finished,           // const VkSemaphore           *pWaitSemaphores
    1,                                            // uint32_t                     swapchainCount
    &swapchain,                            // const VkSwapchainKHR        *pSwapchains
    &image_index,                                 // const uint32_t              *pImageIndices
    NULL                                       // VkResult           *pResults
  };
  VkResult result = vkQueuePresentKHR( gpu_g.gpus[0].queue_families[0].queues[0].queue,
				       &present_info );
  switch(result){
  case VK_SUCCESS:
  case VK_SUBOPTIMAL_KHR:
    break;
  case VK_ERROR_OUT_OF_DATE_KHR:
    //recreate swapchain and command buffers, window has been resized
    printf("khr out of date\n");
    break;
  default:
    printf("ERROR::failed to acquire image\n");
    return -1;
    break;
  }
  
  
  //end of tmp code
  
  run = 1;
  while(run){
    nus_system_events_handle(win);
  }
  
  printf("freeing unit test %s\n", PROGRAM_NAME);

  //tmp cleanup
  
  vkDeviceWaitIdle(gpu_g.gpus[0].logical_device);
  if(image_available != VK_NULL_HANDLE){
    vkDestroySemaphore(gpu_g.gpus[0].logical_device, image_available, NULL);
  }
  if(render_finished != VK_NULL_HANDLE){
    vkDestroySemaphore(gpu_g.gpus[0].logical_device, render_finished, NULL);
  }
  if(swapchain != VK_NULL_HANDLE){
    vkDestroySwapchainKHR(gpu_g.gpus[0].logical_device, swapchain, NULL);
  }
  

  //end tmp cleanup
  
  nus_multi_gpu_free(&gpu_g); 
  nus_presentation_surface_free(vulkan_instance, &present);
  nus_vulkan_instance_free(&vulkan_instance);
  nus_free_vulkan_library();
  
  nus_window_free(&win);
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
void close_win(void)
{
  run = 0;
}
