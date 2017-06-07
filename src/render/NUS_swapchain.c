#include "NUS_swapchain.h"
#include "../gpu/NUS_queue_info.h"
#include <stdio.h>
#include <stdlib.h>

static NUS_result nus_swapchain_build_info
(VkPhysicalDevice, VkSurfaceKHR, NUS_swapchain *);
static NUS_result nus_swapchain_build_format
(VkPhysicalDevice, VkSurfaceKHR, NUS_swapchain *);
static NUS_result nus_swapchain_build_present_mode
(VkPhysicalDevice, VkSurfaceKHR, NUS_swapchain *);
static NUS_result nus_swapchain_build_length(NUS_swapchain *);
static NUS_result nus_swapchain_build_extent(NUS_swapchain *);
static NUS_result nus_swapchain_build_transform_bits(NUS_swapchain *);
static NUS_result nus_swapchain_rebuild
(NUS_gpu, VkSurfaceKHR, NUS_swapchain *);

NUS_result nus_swapchain_build
(NUS_gpu gpu, VkSurfaceKHR surface, NUS_swapchain *p_swapchain)
{
  if(nus_swapchain_build_info(gpu.physical_device, surface, p_swapchain) !=
     NUS_SUCCESS){
    printf("ERROR::failed to obtain swapchain info\n");
    return NUS_FAILURE;
  }
  VkSwapchainCreateInfoKHR swapchain_create_info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = surface,
    .minImageCount = p_swapchain->image_count,
    .imageFormat = p_swapchain->format.format,
    .imageColorSpace = p_swapchain->format.colorSpace,
    .imageExtent = p_swapchain->extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .preTransform = p_swapchain->transform_bits,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = p_swapchain->present_mode,
    .clipped = VK_TRUE,
    .oldSwapchain = VK_NULL_HANDLE
  };
  if(vkCreateSwapchainKHR(gpu.logical_device, &swapchain_create_info, NULL,
			  &p_swapchain->swapchain) != VK_SUCCESS){
    printf("ERROR::failed to create swapchain\n");    
    return NUS_FAILURE;
  }
  // Get count of images that were actually created
  if(vkGetSwapchainImagesKHR(gpu.logical_device, p_swapchain->swapchain,
			     &p_swapchain->image_count, NULL) != VK_SUCCESS ||
     p_swapchain->image_count == 0){
    printf("ERROR::failed to obtain swapchain images: count = %d\n",
	   p_swapchain->image_count);
    return NUS_FAILURE;
  }
  // Get array of created swapchain images
  p_swapchain->images = malloc(sizeof(*p_swapchain->images) *
			       p_swapchain->image_count);
  if(vkGetSwapchainImagesKHR(gpu.logical_device, p_swapchain->swapchain,
			     &p_swapchain->image_count, p_swapchain->images) !=
     VK_SUCCESS){
    printf("ERROR::failed to obtain swapchain images\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
void nus_swapchain_free(NUS_gpu gpu, NUS_swapchain *p_swapchain)
{
  vkDeviceWaitIdle(gpu.logical_device);
  vkDestroySwapchainKHR(gpu.logical_device, p_swapchain->swapchain, NULL);
}
VkImage nus_swapchain_get_image(NUS_swapchain swapchain)
{
  return swapchain.images[swapchain.image_index];
}
NUS_result nus_swapchain_present
(NUS_gpu *gpu, VkSurfaceKHR surface, VkSemaphore wait, NUS_swapchain *p_swapchain)
{
  NUS_queue_info info;
  nus_gpu_find_queue_info(gpu, NUS_QUEUE_FAMILY_SUPPORT_PRESENT, &info);
  
  VkPresentInfoKHR present_info = {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext = NULL,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &wait,
    .swapchainCount = 1,
    .pSwapchains = &p_swapchain->swapchain,
    .pImageIndices = &p_swapchain->image_index,
    .pResults = NULL
  };
  switch(vkQueuePresentKHR(info.p_command_group->queue, &present_info)){
  case VK_SUCCESS:
  case VK_SUBOPTIMAL_KHR:
    break;
  case VK_ERROR_OUT_OF_DATE_KHR:
    nus_swapchain_rebuild(*gpu, surface, p_swapchain);
    break;
  default:
    printf("ERROR::failed to present swapchain image\n");
    return NUS_FAILURE;
    break;
  }
  return NUS_SUCCESS;
}
NUS_result nus_swapchain_new_image
(NUS_gpu gpu, VkSurfaceKHR surface, VkSemaphore signal, NUS_swapchain *p_swapchain)
{
  switch(vkAcquireNextImageKHR(gpu.logical_device, p_swapchain->swapchain, UINT_MAX,
			       signal, VK_NULL_HANDLE,
			       &p_swapchain->image_index)){
  case VK_SUCCESS:
    break;
  case VK_SUBOPTIMAL_KHR:
  case VK_ERROR_OUT_OF_DATE_KHR:
    nus_swapchain_rebuild(gpu, surface, p_swapchain);
    break;
  default:
    printf("ERROR::failed to acquire image\n");
    return NUS_FAILURE;
    break;
  }
  return NUS_SUCCESS; 
}

static NUS_result nus_swapchain_build_info
(VkPhysicalDevice physical_device, VkSurfaceKHR surface, NUS_swapchain *p_swapchain)
{
  if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface,
					       &p_swapchain->capabilities) !=
     VK_SUCCESS){
    printf("ERROR::failed to obtain swapchain capabilities\n");
    return -1;
  }
  if(nus_swapchain_build_length(p_swapchain) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build swapchain length\n");
    return NUS_FAILURE;
  }
  if(nus_swapchain_build_format(physical_device, surface, p_swapchain) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build swapchain formats\n");
    return NUS_FAILURE;
  }
  if(nus_swapchain_build_present_mode(physical_device, surface,
						  p_swapchain) != NUS_SUCCESS){
    printf("ERROR::failed to build swapchain present modes\n");
    return NUS_FAILURE;
  }
  if(nus_swapchain_build_extent(p_swapchain) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build swapchain extent\n");
    return NUS_FAILURE;
  }
  if(nus_swapchain_build_transform_bits(p_swapchain) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build swapchain transform bits\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_swapchain_build_format
(VkPhysicalDevice physical_device, VkSurfaceKHR surface, NUS_swapchain *p_swapchain)
{
  unsigned int i,
    format_count;
  if((vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface,
					   &format_count, NULL) != VK_SUCCESS) ||
     (format_count == 0)){
    printf("ERROR::failed to obtain surface format count\n");
    return NUS_FAILURE;
  }
  VkSurfaceFormatKHR available_formats[format_count];
  if((vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count,
					   available_formats) != VK_SUCCESS)){
    printf("ERROR::failed to obtain surface formats\n");
    return NUS_FAILURE;
  }
  if((1 == format_count) && (VK_FORMAT_UNDEFINED == available_formats[0].format)){
    // If the surface has no preferred format
    p_swapchain->format = (VkSurfaceFormatKHR)
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
    p_swapchain->format = available_formats[desired_format_index];
  }
  return NUS_SUCCESS;
}
static NUS_result nus_swapchain_build_present_mode
(VkPhysicalDevice physical_device, VkSurfaceKHR surface, NUS_swapchain *p_swapchain)
{
  unsigned int i,
    present_mode_count;
  if((vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface,
						&present_mode_count,
						NULL) != VK_SUCCESS) ||
     (present_mode_count == 0)){
    printf("ERROR::failed to obtain surface present mode count\n");
    return NUS_FAILURE;
  }
  VkPresentModeKHR available_present_modes[present_mode_count];
  if(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface,
					       &present_mode_count,
					       available_present_modes) !=
     VK_SUCCESS){
    printf("ERROR::failed to obtain surface present modes\n");
    return NUS_FAILURE;
  }
  
  unsigned int desired_present_mode_index = UINT_MAX;
  if(p_swapchain->image_count > 3){
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
    printf("ERROR::present mode FIFO is not supported: That should not be possible\n");
    return NUS_FAILURE;
  }
  p_swapchain->present_mode = available_present_modes[desired_present_mode_index];
  return NUS_SUCCESS;
}
static NUS_result nus_swapchain_build_length
(NUS_swapchain *p_swapchain)
{
  if(0 == p_swapchain->capabilities.maxImageCount){
    // If there is no max limit on number of images

    p_swapchain->image_count = NUS_SWAPCHAIN_DESIRED_IMAGE_COUNT;
  }else if((NUS_SWAPCHAIN_DESIRED_IMAGE_COUNT >
	    p_swapchain->capabilities.minImageCount) &&
	   (NUS_SWAPCHAIN_DESIRED_IMAGE_COUNT <
	    p_swapchain->capabilities.maxImageCount)){
    p_swapchain->image_count = NUS_SWAPCHAIN_DESIRED_IMAGE_COUNT;
  }else{
    p_swapchain->image_count =
      p_swapchain->capabilities.minImageCount + 1;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_swapchain_build_extent
(NUS_swapchain *p_swapchain)
{
  // Obtain desired size of swapchain images
  if(-1 != p_swapchain->capabilities.currentExtent.width){
    // If there is no limit on the surface extent
    p_swapchain->extent = p_swapchain->capabilities.currentExtent;
  } else{
    // Else set to the max allowed extent
    p_swapchain->extent = (VkExtent2D){
      p_swapchain->capabilities.maxImageExtent.width,
      p_swapchain->capabilities.maxImageExtent.height
    };
  }
  return NUS_SUCCESS;
}
static NUS_result nus_swapchain_build_transform_bits
(NUS_swapchain *p_swapchain)
{
  if(p_swapchain->capabilities.supportedTransforms &
     VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR){
    p_swapchain->transform_bits =
      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  }else{
    p_swapchain->transform_bits =
      p_swapchain->capabilities.currentTransform;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_swapchain_rebuild
(NUS_gpu gpu, VkSurfaceKHR surface, NUS_swapchain *p_swapchain)
{
  //TODO faster to wait on fences of few commands that operate on swaochain images
  vkDeviceWaitIdle(gpu.logical_device);
  nus_swapchain_free(gpu, p_swapchain);
  if(nus_swapchain_build(gpu, surface, p_swapchain) != NUS_SUCCESS){
    printf("ERROR::failed to rebuild swapchain: build failed\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
