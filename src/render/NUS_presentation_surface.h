#ifndef _NUS_PRESENTATION_SURFACE_H_
#define _NUS_PRESENTATION_SURFACE_H_

#include "../NUS_engine.h"
#include <vulkan/vulkan.h>
#include "../NUS_result.h"

struct NUS_window;
struct NUS_vulkan_instance;

typedef struct NUS_presentation_surface{
  VkSurfaceKHR surface;
} NUS_presentation_surface;

NUS_result nus_build_presentation_surface
(struct NUS_window, struct NUS_vulkan_instance, NUS_presentation_surface *);
void nus_free_presentation_surface(NUS_presentation_surface *);

#endif /* _NUS_PRESENTATION_SURFACE_H_ */
