#ifndef _NUS_PRESENTATION_SURFACE_H_
#define _NUS_PRESENTATION_SURFACE_H_

#include "../NUS_result.h"
#include "../NUS_vulkan.h"

struct NUS_window;
struct NUS_vulkan_instance;
struct NUS_multi_gpu;

typedef struct NUS_presentation_surface{
  VkSurfaceKHR surface;
  VkSwapchainKHR swapchain;
} NUS_presentation_surface;

NUS_result nus_presentation_surface_build
(struct NUS_window, struct NUS_vulkan_instance, struct NUS_multi_gpu *,
 NUS_presentation_surface *);
void nus_presentation_surface_free
(struct NUS_vulkan_instance, NUS_presentation_surface *);

#endif /* _NUS_PRESENTATION_SURFACE_H_ */
