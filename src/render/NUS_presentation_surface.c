#include "NUS_presentation_surface.h"
#include "../io/NUS_window.h"
#include "../gpu/NUS_multi_gpu.h"
#include "../gpu/NUS_vulkan_instance.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static NUS_result nus_presentation_surface_build_swapchain
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_new_image
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_swapchain_info
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_update
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_swapchain_length
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_surface_formats
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_surface_present_modes
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_surface_extent
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_surface_transform_bits
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

  unsigned int suitable_gpu_index;
  if(nus_multi_gpu_find_suitable_gpu(*NUS_multi_gpu_,
				     NUS_QUEUE_FAMILY_SUPPORT_PRESENT |
				     NUS_QUEUE_FAMILY_SUPPORT_TRANSFER,
				     &suitable_gpu_index) != NUS_SUCCESS){
    printf("ERROR::failed to find presentation surface suitable gpu index\n");
    return NUS_FAILURE;
  }
  p_presentation_surface->presenting_gpu =
    NUS_multi_gpu_->gpus + suitable_gpu_index;
  
  nus_bind_device_vulkan_library(p_presentation_surface->presenting_gpu->functions);

  if(nus_presentation_surface_build_swapchain_info(p_presentation_surface) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build presentation surface swapchain info\n");
    return NUS_FAILURE;
  }
  p_presentation_surface->swapchain = VK_NULL_HANDLE;
  if(nus_presentation_surface_build_swapchain(p_presentation_surface) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build presentation surface swapchain\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
void nus_presentation_surface_free
(NUS_vulkan_instance NUS_vulkan_instance_,
 NUS_presentation_surface *p_presentation_surface)
{
  vkDeviceWaitIdle(p_presentation_surface->presenting_gpu->logical_device);
  if(p_presentation_surface->image_available != VK_NULL_HANDLE){
    vkDestroySemaphore(p_presentation_surface->presenting_gpu->logical_device,
		       p_presentation_surface->image_available, NULL);
  }
  if(p_presentation_surface->image_rendered != VK_NULL_HANDLE){
    vkDestroySemaphore(p_presentation_surface->presenting_gpu->logical_device,
		       p_presentation_surface->image_rendered, NULL);
  }
  vkDestroySwapchainKHR(p_presentation_surface->presenting_gpu->logical_device,
			p_presentation_surface->swapchain, NULL);
  p_presentation_surface->presenting_gpu = NULL;
  p_presentation_surface->swapchain_length = 0;
  p_presentation_surface->image_index = UINT_MAX;
  vkDestroySurfaceKHR(NUS_vulkan_instance_.instance,
		      p_presentation_surface->surface, NULL);
}
NUS_result nus_presentation_surface_present
(NUS_presentation_surface *p_presentation_surface)
{
  unsigned int queue_family_index = UINT_MAX,
    queue_index = UINT_MAX;
  if((nus_gpu_find_suitable_queue_family(*p_presentation_surface->presenting_gpu,
					 NUS_QUEUE_FAMILY_SUPPORT_PRESENT,
					 &queue_family_index) != NUS_SUCCESS) ||
     UINT_MAX == queue_family_index){
    printf("ERROR::failed to find suitable queue family for presentation\n");
    return NUS_FAILURE;
  }
  if((nus_queue_family_find_suitable_queue(p_presentation_surface->presenting_gpu->
					   queue_families[queue_family_index],
					   &queue_index) != NUS_SUCCESS) ||
     UINT_MAX == queue_index){
    printf("ERROR::failed to find suitable queue for presentation\n");
    return NUS_FAILURE;
  }
  VkPresentInfoKHR present_info = {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext = NULL,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &p_presentation_surface->image_rendered,
    .swapchainCount = 1,
    .pSwapchains = &p_presentation_surface->swapchain,
    .pImageIndices = &p_presentation_surface->image_index,
    .pResults = NULL
  };
  switch(vkQueuePresentKHR(p_presentation_surface->presenting_gpu->
			   queue_families[queue_family_index].
			   queues[queue_index].queue, &present_info)){
  case VK_SUCCESS:
  case VK_SUBOPTIMAL_KHR:
    break;
  case VK_ERROR_OUT_OF_DATE_KHR:
    nus_presentation_surface_update(p_presentation_surface);
    break;
  default:
    printf("ERROR::failed to present image\n");
    return NUS_FAILURE;
    break;
  }
  if(nus_presentation_surface_new_image(p_presentation_surface) != NUS_SUCCESS){
    printf("ERROR::failed to get new swapchain image\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_swapchain
(NUS_presentation_surface *p_presentation_surface)
{
  VkSwapchainCreateInfoKHR swapchain_create_info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = p_presentation_surface->surface,
    .minImageCount = p_presentation_surface->swapchain_length,
    .imageFormat = p_presentation_surface->format.format,
    .imageColorSpace = p_presentation_surface->format.colorSpace,
    .imageExtent = p_presentation_surface->extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .preTransform = p_presentation_surface->transform_bits,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = p_presentation_surface->present_mode,
    .clipped = VK_TRUE,
    .oldSwapchain = p_presentation_surface->swapchain
  };
  if(vkCreateSwapchainKHR(p_presentation_surface->presenting_gpu->logical_device,
			  &swapchain_create_info, NULL,
			  &p_presentation_surface->swapchain) != VK_SUCCESS){
    printf("ERROR::failed to create swapchain\n");    
    return NUS_FAILURE;
  }

  // Get count of images that were actually created
  if(vkGetSwapchainImagesKHR(p_presentation_surface->presenting_gpu->logical_device,
			     p_presentation_surface->swapchain,
			     &p_presentation_surface->swapchain_length,
			     NULL) != VK_SUCCESS ||
     p_presentation_surface->swapchain_length == 0){
    printf("ERROR::failed to obtain swapchain image count: count = %d\n",
	   p_presentation_surface->swapchain_length);
    return NUS_FAILURE;
  }
  // Get array of created swapchain images
  p_presentation_surface->swapchain_images =
    malloc(sizeof(*p_presentation_surface->swapchain_images) *
	   p_presentation_surface->swapchain_length);
  if(vkGetSwapchainImagesKHR(p_presentation_surface->presenting_gpu->logical_device,
			     p_presentation_surface->swapchain,
			     &p_presentation_surface->swapchain_length,
			     p_presentation_surface->swapchain_images) !=
     VK_SUCCESS){
    printf("ERROR::failed to obtain swapchain images\n");
    return NUS_FAILURE;
  } 
  // Create semaphores vital to managing render_image access
  VkSemaphoreCreateInfo image_available_create_info = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0
  };
  VkSemaphoreCreateInfo image_rendered_create_info = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0
  };
  if(vkCreateSemaphore(p_presentation_surface->presenting_gpu->logical_device,
		       &image_available_create_info, NULL,
		       &p_presentation_surface->image_available) !=
     VK_SUCCESS){
    printf("ERROR::failed to create surface semaphore: image_available\n");
    return NUS_FAILURE;
  }
  if(vkCreateSemaphore(p_presentation_surface->presenting_gpu->logical_device,
		       &image_rendered_create_info, NULL,
		       &p_presentation_surface->image_rendered) !=
     VK_SUCCESS){
    printf("ERROR::failed to create surface semaphore: image_rendered\n");
    return NUS_FAILURE;
  }
  
  nus_presentation_surface_new_image(p_presentation_surface);
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_new_image
(NUS_presentation_surface *p_presentation_surface)
{\
  switch(vkAcquireNextImageKHR(p_presentation_surface->presenting_gpu->
			       logical_device,
			       p_presentation_surface->swapchain,
			       UINT_MAX,
			       p_presentation_surface->image_available,
			       VK_NULL_HANDLE,
			       &p_presentation_surface->image_index)){
  case VK_SUCCESS:
    break;
  case VK_SUBOPTIMAL_KHR:
  case VK_ERROR_OUT_OF_DATE_KHR:
    nus_presentation_surface_update(p_presentation_surface);
    break;
  default:
    printf("ERROR::failed to acquire image\n");
    return NUS_FAILURE;
    break;
  }
  p_presentation_surface->render_image =
    p_presentation_surface->swapchain_images[p_presentation_surface->image_index];
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_swapchain_info
(NUS_presentation_surface *p_presentation_surface)
{
  if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(p_presentation_surface->
					       presenting_gpu->physical_device,
					       p_presentation_surface->surface,
					       &p_presentation_surface->
					       capabilities) !=
     VK_SUCCESS){
    printf("ERROR::failed to obtain surface capabilities\n");
    return -1;
  }
  if(nus_presentation_surface_build_swapchain_length(p_presentation_surface) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build swapchain length\n");
    return NUS_FAILURE;
  }
  if(nus_presentation_surface_build_surface_formats(p_presentation_surface) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build surface formats\n");
    return NUS_FAILURE;
  }
  if(nus_presentation_surface_build_surface_present_modes(p_presentation_surface) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build surface present modes\n");
    return NUS_FAILURE;
  }
  if(nus_presentation_surface_build_surface_extent(p_presentation_surface) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build surface extent\n");
    return NUS_FAILURE;
  }
  if(nus_presentation_surface_build_surface_transform_bits(p_presentation_surface) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build surface transform bits\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_update
(NUS_presentation_surface *p_presentation_surface)
{
  vkDeviceWaitIdle(p_presentation_surface->presenting_gpu->logical_device);
  if(nus_presentation_surface_build_swapchain_info(p_presentation_surface) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build presentation surface swapchain info\n");
    return NUS_FAILURE;
  }
  if(nus_presentation_surface_build_swapchain(p_presentation_surface) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build presentation surface swapchain\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_swapchain_length
(NUS_presentation_surface *p_presentation_surface)
{
  if(0 == p_presentation_surface->capabilities.maxImageCount){
    // If there is no max limit on number of images

    // 3 is our optimal length for mailbox present mode
    p_presentation_surface->swapchain_length = 3;
  }else if((4 > p_presentation_surface->capabilities.minImageCount) &&
	   (4 < p_presentation_surface->capabilities.maxImageCount)){
    p_presentation_surface->swapchain_length = 3;
  }else{
    p_presentation_surface->swapchain_length =
      p_presentation_surface->capabilities.minImageCount + 1;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_surface_formats
(NUS_presentation_surface *p_presentation_surface)
{
  unsigned int i,
    format_count;
  
  if((vkGetPhysicalDeviceSurfaceFormatsKHR(p_presentation_surface->
					   presenting_gpu->physical_device,
					   p_presentation_surface->surface,
					   &format_count,
					   NULL) != VK_SUCCESS) ||
     (format_count == 0)){
    printf("ERROR::failed to obtain surface format count\n");
    return NUS_FAILURE;
  }
  VkSurfaceFormatKHR available_formats[format_count];
  if((vkGetPhysicalDeviceSurfaceFormatsKHR(p_presentation_surface->
					   presenting_gpu->physical_device,
					   p_presentation_surface->surface,
					   &format_count,
					   available_formats) != VK_SUCCESS)){
    printf("ERROR::failed to obtain surface formats\n");
    return NUS_FAILURE;
  }
  if((1 == format_count) && (VK_FORMAT_UNDEFINED == available_formats[0].format)){
    // If the surface has no preferred format
    p_presentation_surface->format = (VkSurfaceFormatKHR)
      {VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR};
  } else{
    // Search available formats for our preferred 
    unsigned int desired_format_index = 0;
    for(i = 0; i < format_count; ++i){
      if(VK_FORMAT_R8G8B8A8_UNORM == available_formats[i].format &&
	 VK_COLORSPACE_SRGB_NONLINEAR_KHR == available_formats[i].colorSpace){
	// If the current format is our preferred
	desired_format_index = i;
	break;
      }
    }
    // In the situation that the desired format is not found
    //   the format at index 0 is used
    p_presentation_surface->format = available_formats[desired_format_index];
  }
  return NUS_SUCCESS;
}

static NUS_result nus_presentation_surface_build_surface_present_modes
(NUS_presentation_surface *p_presentation_surface)
{
  unsigned int i,
    present_mode_count;
  if((vkGetPhysicalDeviceSurfacePresentModesKHR(p_presentation_surface->
						presenting_gpu->physical_device,
						p_presentation_surface->surface,
						&present_mode_count,
						NULL) != VK_SUCCESS) ||
     (present_mode_count == 0)){
    printf("ERROR::failed to obtain surface present mode count\n");
    return NUS_FAILURE;
  }
  VkPresentModeKHR available_present_modes[present_mode_count];
  if(vkGetPhysicalDeviceSurfacePresentModesKHR(p_presentation_surface->
					       presenting_gpu->physical_device,
					       p_presentation_surface->surface,
					       &present_mode_count,
					       available_present_modes) !=
     VK_SUCCESS){
    printf("ERROR::failed to obtain surface present modes\n");
    return NUS_FAILURE;
  }
  
  unsigned int desired_present_mode_index = UINT_MAX;
  if(p_presentation_surface->swapchain_length > 3){
    for(i = 0; i < present_mode_count; ++i){
      // Search for our desired present mode
      if(VK_PRESENT_MODE_MAILBOX_KHR == available_present_modes[i]){
	desired_present_mode_index = i;
	break;
      }
    }
  }
  if(UINT_MAX == desired_present_mode_index){
    // If our desired present mode has not been found
    for(i = 0; i < present_mode_count; ++i){
      if(VK_PRESENT_MODE_FIFO_KHR == available_present_modes[i]){
	// Present mode FIFO will always be supported
	desired_present_mode_index = i;
	break;
      }
    }
  }
  if(UINT_MAX == desired_present_mode_index){
    printf("ERROR::present mode FIFO is not supported\n");
    printf("\tThat should not be possible\n");
    return NUS_FAILURE;
  }
  p_presentation_surface->present_mode =
    available_present_modes[desired_present_mode_index];
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_surface_extent
(NUS_presentation_surface *p_presentation_surface)
{
  // Obtain desired size of swapchain images
  if(-1 != p_presentation_surface->capabilities.currentExtent.width){
    // If there is no limit on the surface extent
    p_presentation_surface->extent =
      p_presentation_surface->capabilities.currentExtent;
  } else{
    // Else set to the max allowed extent
    p_presentation_surface->extent = (VkExtent2D){
      p_presentation_surface->capabilities.maxImageExtent.width,
      p_presentation_surface->capabilities.maxImageExtent.height
    };
  }
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_surface_transform_bits
(NUS_presentation_surface *p_presentation_surface)
{
  if(p_presentation_surface->capabilities.supportedTransforms &
     VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR){
    p_presentation_surface->transform_bits =
      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  }else{
    p_presentation_surface->transform_bits =
      p_presentation_surface->capabilities.currentTransform;
  }
  return NUS_SUCCESS;
}
