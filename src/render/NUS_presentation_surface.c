#include "NUS_presentation_surface.h"
#include "../io/NUS_window.h"
#include "../gpu/NUS_multi_gpu.h"
#include "../gpu/NUS_vulkan_instance.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static NUS_result nus_presentation_surface_build_swapchain
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_swapchain_info
(VkPhysicalDevice, NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_swapchain_length
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_surface_formats
(VkPhysicalDevice, NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_surface_present_modes
(VkPhysicalDevice, NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_surface_extent
(NUS_presentation_surface *);
static NUS_result nus_presentation_surface_build_surface_transform_bits
(NUS_presentation_surface *);


NUS_result nus_presentation_surface_build
(NUS_window NUS_window_, NUS_vulkan_instance NUS_vulkan_instance_,
 NUS_multi_gpu *NUS_multi_gpu_,
 NUS_presentation_surface *NUS_presentation_surface_)
{

#if defined(NUS_OS_WINDOWS)
  VkWin32SurfaceCreateInfoKHR surface_create_info;
  surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  surface_create_info.pNext = NULL;
  surface_create_info.flags = 0;
  //surface_create_info.hinstance = Window.Instance;//TODO support windows
  //surface_create_info.hwind = Window.Handle;
  if(vkCreateWin32SurfaceKHR(NUS_vulkan_instance_.instance, &surface_create_info,
			      NULL, &NUS_presentation_surface_.surface) != VK_SUCCESS){
    printf("ERROR::unable to create Win32 vulkan surface\n");
    return NUS_FAILURE;
  }
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  VkXcbSurfaceCreateInfoKHR surface_create_info;
  surface_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
  surface_create_info.pNext = NULL;
  surface_create_info.flags = 0;
  surface_create_info.connection = NUS_window_.connection;
  surface_create_info.window = NUS_window_.window;
  if(vkCreateXcbSurfaceKHR(NUS_vulkan_instance_.instance, &surface_create_info,
			   NULL, &NUS_presentation_surface_->surface) != VK_SUCCESS){
    printf("ERROR::unable to create XCB vulkan surface\n");
    return NUS_FAILURE;
  }
#endif
  nus_multi_gpu_check_surface_support(NUS_presentation_surface_->surface,
				      NUS_multi_gpu_);

  unsigned int suitable_gpu_index;
  if(nus_multi_gpu_find_suitable_gpu_index(*NUS_multi_gpu_,
					   &suitable_gpu_index) != NUS_SUCCESS){
    printf("ERROR::failed to find presentation surface suitable gpu index\n");
    return NUS_FAILURE;
  }
  NUS_gpu * const suitable_gpu = NUS_multi_gpu_->gpus + suitable_gpu_index;
  NUS_presentation_surface_->presenting_device = suitable_gpu->logical_device;
  nus_bind_device_vulkan_library(suitable_gpu->functions);
  VkPhysicalDevice suitable_physical_device =
    NUS_multi_gpu_->physical_devices[suitable_gpu_index];
  
  if(nus_presentation_surface_build_swapchain_info(suitable_physical_device,
						   NUS_presentation_surface_) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build presentation surface swapchain info\n");
    return NUS_FAILURE;
  }
  NUS_presentation_surface_->swapchain = VK_NULL_HANDLE;
  if(nus_presentation_surface_build_swapchain(NUS_presentation_surface_) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build presentation surface swapchain\n");
    return NUS_FAILURE;
  }
  
  
  return NUS_SUCCESS;
}
void nus_presentation_surface_free
(NUS_vulkan_instance NUS_vulkan_instance_,
 NUS_presentation_surface *NUS_presentation_surface_)
{
  vkDestroySwapchainKHR(NUS_presentation_surface_->presenting_device,
			NUS_presentation_surface_->swapchain, NULL);
  vkDestroySurfaceKHR(NUS_vulkan_instance_.instance,
		      NUS_presentation_surface_->surface, NULL);
}
void nus_presentation_surface_clear
(unsigned int image_index, VkClearColorValue clear_color,
 NUS_presentation_surface *NUS_presentation_surface_)
{
  // submit information to queues to clear image
}
static NUS_result nus_presentation_surface_build_swapchain
(NUS_presentation_surface *NUS_presentation_surface_)
{
  VkSwapchainCreateInfoKHR swapchain_create_info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = NUS_presentation_surface_->surface,
    .minImageCount = NUS_presentation_surface_->swapchain_length,
    .imageFormat = NUS_presentation_surface_->format.format,
    .imageColorSpace = NUS_presentation_surface_->format.colorSpace,
    .imageExtent = NUS_presentation_surface_->extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .preTransform = NUS_presentation_surface_->transform_bits,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = NUS_presentation_surface_->present_mode,
    .clipped = VK_TRUE,
    .oldSwapchain = NUS_presentation_surface_->swapchain
  };
  if(vkCreateSwapchainKHR(NUS_presentation_surface_->presenting_device,
			  &swapchain_create_info, NULL,
			  &NUS_presentation_surface_->swapchain) != VK_SUCCESS){
    printf("ERROR::failed to create swapchain\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_swapchain_info
(VkPhysicalDevice suitable_physical_device,
 NUS_presentation_surface *NUS_presentation_surface_)
{
  
  if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(suitable_physical_device,
					       NUS_presentation_surface_->surface,
					       &NUS_presentation_surface_->
					       capabilities) !=
     VK_SUCCESS){
    printf("ERROR::failed to obtain surface capabilities\n");
    return -1;
  }
  
  if(nus_presentation_surface_build_swapchain_length(NUS_presentation_surface_) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build swapchain length\n");
    return NUS_FAILURE;
  }
  if(nus_presentation_surface_build_surface_formats(suitable_physical_device,
						    NUS_presentation_surface_) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build surface formats\n");
    return NUS_FAILURE;
  }
  if(nus_presentation_surface_build_surface_present_modes(suitable_physical_device,
							  NUS_presentation_surface_) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build surface present modes\n");
    return NUS_FAILURE;
  }
  if(nus_presentation_surface_build_surface_extent(NUS_presentation_surface_) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build surface extent\n");
    return NUS_FAILURE;
  }
  if(nus_presentation_surface_build_surface_transform_bits(NUS_presentation_surface_) !=
     NUS_SUCCESS){
    printf("ERROR::failed to build surface transform bits\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_swapchain_length
(NUS_presentation_surface *NUS_presentation_surface_)
{
  if(0 == NUS_presentation_surface_->capabilities.maxImageCount){
    // If there is no max limit on number of images

    // 3 is our optimal length for mailbox present mode
    NUS_presentation_surface_->swapchain_length = 3;
  }else if((3 > NUS_presentation_surface_->capabilities.minImageCount) &&
	   (3 < NUS_presentation_surface_->capabilities.maxImageCount)){
    NUS_presentation_surface_->swapchain_length = 3;
  }else{
    NUS_presentation_surface_->swapchain_length =
      NUS_presentation_surface_->capabilities.minImageCount + 1;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_surface_formats
(VkPhysicalDevice physical_device,
 NUS_presentation_surface *NUS_presentation_surface_)
{
  unsigned int i,
    format_count;
  
  if((vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
					   NUS_presentation_surface_->surface,
					   &format_count,
					   NULL) != VK_SUCCESS) ||
     (format_count == 0)){
    printf("ERROR::failed to obtain surface format count\n");
    return NUS_FAILURE;
  }
  VkSurfaceFormatKHR available_formats[format_count];
  if((vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
					   NUS_presentation_surface_->surface,
					   &format_count,
					   available_formats) != VK_SUCCESS)){
    printf("ERROR::failed to obtain surface formats\n");
    return NUS_FAILURE;
  }
  if((1 == format_count) && (VK_FORMAT_UNDEFINED == available_formats[0].format)){
    // If the surface has no preferred format
    NUS_presentation_surface_->format = (VkSurfaceFormatKHR)
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
    // If desired format is not found the format at index 0 is used
    NUS_presentation_surface_->format = available_formats[desired_format_index];
  }
  return NUS_SUCCESS;
}

static NUS_result nus_presentation_surface_build_surface_present_modes
(VkPhysicalDevice physical_device,
 NUS_presentation_surface *NUS_presentation_surface_)
{
  unsigned int i,
    present_mode_count;
  if((vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
						NUS_presentation_surface_->surface,
						&present_mode_count,
						NULL) != VK_SUCCESS) ||
     (present_mode_count == 0)){
    printf("ERROR::failed to obtain surface present mode count\n");
    return NUS_FAILURE;
  }
  VkPresentModeKHR available_present_modes[present_mode_count];
  if(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
					       NUS_presentation_surface_->surface,
					       &present_mode_count,
					       available_present_modes) !=
     VK_SUCCESS){
    printf("ERROR::failed to obtain surface present modes\n");
    return NUS_FAILURE;
  }
  
  unsigned int desired_present_mode_index = UINT_MAX;
  for(i = 0; i < present_mode_count; ++i){
    // Search for our desired present mode
    if(VK_PRESENT_MODE_MAILBOX_KHR == available_present_modes[i]){
      desired_present_mode_index = i;
      break;
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
  NUS_presentation_surface_->present_mode =
    available_present_modes[desired_present_mode_index];
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_surface_extent
(NUS_presentation_surface *NUS_presentation_surface_)
{
  /* Obtain desired size of swapchain images */
  if(-1 != NUS_presentation_surface_->capabilities.currentExtent.width){
    // If there is no limit on the surface extent
    NUS_presentation_surface_->extent =
      NUS_presentation_surface_->capabilities.currentExtent;
  } else{
    // Else set to the max allowed extent
    NUS_presentation_surface_->extent = (VkExtent2D){
      NUS_presentation_surface_->capabilities.maxImageExtent.width,
      NUS_presentation_surface_->capabilities.maxImageExtent.height
    };
  }
  return NUS_SUCCESS;
}
static NUS_result nus_presentation_surface_build_surface_transform_bits
(NUS_presentation_surface *NUS_presentation_surface_)
{
  if(NUS_presentation_surface_->capabilities.supportedTransforms &
     VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR){
    NUS_presentation_surface_->transform_bits =
      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  }else{
    NUS_presentation_surface_->transform_bits =
      NUS_presentation_surface_->capabilities.currentTransform;
  }
  return NUS_SUCCESS;
}
