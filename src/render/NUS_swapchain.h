#ifndef NUS_SWAPCHAIN_H
#define NUS_SWAPCHAIN_H

#define NUS_SWAPCHAIN_DESIRED_IMAGE_COUNT 3

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "../gpu/NUS_gpu.h"

typedef struct NUS_swapchain{
  VkSwapchainKHR swapchain;
  VkImage *images;
  VkExtent2D extent;
  VkSurfaceFormatKHR format;
  VkSurfaceCapabilitiesKHR capabilities;
  VkPresentModeKHR present_mode;
  VkSurfaceTransformFlagBitsKHR transform_bits;
  unsigned int image_count,
    image_index;
} NUS_swapchain;

NUS_result nus_swapchain_build(NUS_gpu, VkSurfaceKHR, NUS_swapchain *);
void nus_swapchain_free(NUS_gpu, NUS_swapchain *);
NUS_result nus_swapchain_present(NUS_gpu *, VkSurfaceKHR, VkSemaphore, NUS_swapchain *);
NUS_result nus_swapchain_new_image(NUS_gpu, VkSurfaceKHR, VkSemaphore, NUS_swapchain *);

#endif /* NUS_SWAPCHAIN_H */
/* partial code for printing swapchain
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
