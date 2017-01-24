#include "NUS_presentation_surface.h"
#include "../io/NUS_window.h"
#include "../gpu/NUS_vulkan_instance.h"
#include <stdio.h>

#if defined(NUS_OS_WINDOWS)

#elif defined(NUS_OS_UNIX)
#include <X11/Xlib-xcb.h>
#endif

NUS_result nus_build_presentation_surface
(NUS_window NUS_window_, NUS_vulkan_instance NUS_vulkan_instance_,
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
  
  return NUS_SUCCESS;
}
void nus_free_presentation_surface
(NUS_vulkan_instance NUS_vulkan_instance_,
 NUS_presentation_surface *NUS_presentation_surface_)
{
  vkDestroySurfaceKHR(NUS_vulkan_instance_.instance,
		      NUS_presentation_surface_->surface, NULL);
}
