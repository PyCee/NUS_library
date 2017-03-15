#ifndef NUS_PRESENTATION_SURFACE_H
#define NUS_PRESENTATION_SURFACE_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "../gpu/NUS_gpu.h"

struct NUS_window;
struct NUS_vulkan_instance;
struct NUS_multi_gpu;

typedef struct NUS_presentation_surface{
  VkSurfaceKHR surface;
  VkSwapchainKHR swapchain;
  NUS_gpu *presenting_gpu;
  VkSurfaceFormatKHR format;
  VkSurfaceCapabilitiesKHR capabilities;
  VkPresentModeKHR present_mode;
  VkSurfaceTransformFlagBitsKHR transform_bits;
  VkExtent2D extent;
  VkImage render_image,
    *swapchain_images;
  VkSemaphore image_available,
    image_rendered,
    render_copied;
  unsigned int swapchain_length,
    image_index;
} NUS_presentation_surface;

NUS_result nus_presentation_surface_build
(struct NUS_window, struct NUS_vulkan_instance, struct NUS_multi_gpu *,
 NUS_presentation_surface *);
void nus_presentation_surface_free
(struct NUS_vulkan_instance, NUS_presentation_surface *);
NUS_result nus_presentation_surface_present(NUS_presentation_surface *);


#endif /* NUS_PRESENTATION_SURFACE_H */
/* partial code for printing presentation surface
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
  
  */
