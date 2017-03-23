#ifndef NUS_PRESENTATION_SURFACE_H
#define NUS_PRESENTATION_SURFACE_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "../gpu/NUS_gpu.h"
#include "../gpu/NUS_queue_info.h"
#include "NUS_swapchain.h"
#include "NUS_texture.h"

struct NUS_window;
struct NUS_vulkan_instance;
struct NUS_multi_gpu;

typedef struct NUS_presentation_surface{
  VkSurfaceKHR surface;
  NUS_swapchain swapchain;
  NUS_queue_info queue_info;
  NUS_texture render_target;
  VkSemaphore render_copied,
    image_available,
    image_presentable;
  VkCommandBuffer *render_target_copy_command_buffers;
} NUS_presentation_surface;

NUS_result nus_presentation_surface_build
(struct NUS_window, struct NUS_vulkan_instance, struct NUS_multi_gpu *,
 NUS_presentation_surface *);
void nus_presentation_surface_free
(struct NUS_vulkan_instance, NUS_presentation_surface *);
NUS_result nus_presentation_surface_present(NUS_presentation_surface *);
void nus_presentation_surface_copy_to_swapchain(NUS_presentation_surface *);

#endif /* NUS_PRESENTATION_SURFACE_H */
