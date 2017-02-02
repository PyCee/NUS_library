#ifndef NUS_PRESENTATION_SURFACE_H
#define NUS_PRESENTATION_SURFACE_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"

struct NUS_window;
struct NUS_vulkan_instance;
struct NUS_multi_gpu;

typedef struct NUS_presentation_surface{
  VkSurfaceKHR surface;
  VkSwapchainKHR swapchain;
  
  VkDevice presenting_device;
  VkSurfaceFormatKHR format;
  VkSurfaceCapabilitiesKHR capabilities;
  VkPresentModeKHR present_mode;
  VkSurfaceTransformFlagBitsKHR transform_bits;
  VkExtent2D extent;
  unsigned int swapchain_length;
} NUS_presentation_surface;

NUS_result nus_presentation_surface_build
(struct NUS_window, struct NUS_vulkan_instance, struct NUS_multi_gpu *,
 NUS_presentation_surface *);
void nus_presentation_surface_free
(struct NUS_vulkan_instance, NUS_presentation_surface *);
void nus_presentation_surface_clear
(unsigned int, VkClearColorValue, NUS_presentation_surface *);


/*  clear color steps
create command pool
get image
create command buffers
create buffer begin
create image subresource range
record command buffer
  2 memory barriers
  record command buffers
submit queue
present image
*/
#endif /* NUS_PRESENTATION_SURFACE_H */
