#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR /* tells vulkan to use win-api extensions */
#elif __unix__
//XLIB vulkan library not supported by my intel graphics card yet
//#define VK_USE_PLATFORM_XLIB_KHR /* tells vulkan to use X11 extensions */
#define VK_USE_PLATFORM_XCB_KHR /* tells vulkan to use XCB extensions */
#endif

#include <stdio.h>
#include <string.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#include <NUS/NUS_engine.h>
#include <vulkan/vulkan.h>
#include <limits.h>

#define PROGRAM_NAME "unit_test-vulkan_clear"

#include <X11/Xlib-xcb.h>

char run;

void close_win(void);

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  unsigned int i,
    j;
  
  NUS_gpu_group gpu_g;

  NUS_window win = nus_build_window(PROGRAM_NAME, 600, 400);
  
  nus_setup_system_events(win);
  NUS_event_handler eve = nus_build_event_handler();
  eve.close_window = close_win;
  nus_set_event_handler(&eve);
  run = 1;

  char *instance_extensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME
    #endif
  };

  NUS_vulkan_instance_info instance_info;
  if(nus_build_vulkan_instance_info(instance_extensions, 2, NULL, 0, &instance_info)
     != NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }
  NUS_vulkan_instance vulkan_instance;
  if(nus_build_vulkan_instance(instance_info, &vulkan_instance)
     != NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }
  nus_free_vulkan_instance_info(&instance_info);
  

  VkSurfaceKHR vulkan_surface;
  
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  VkWin32SurfaceCreateInfoKHR surface_create_info;
  surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  surface_create_info.pNext = NULL;
  surface_create_info.flags = 0;
  //surface_create_info.hinstance = Window.Instance;
  //surface_create_info.hwind = Window.Handle;
  if(vkCreateWin32SurfaceKHR(vulkan_instance, &surface_create_info,
			      NULL, &vulkan_surface) != VK_SUCCESS){
    printf("ERROR::unable to create Win32 vulkan surface\n");
    return -1;
  }
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  
  //must use for unix because my machne doesn't support xlib surface
  VkXcbSurfaceCreateInfoKHR surface_create_info;
  surface_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
  surface_create_info.pNext = NULL;
  surface_create_info.flags = 0;
  surface_create_info.connection = XGetXCBConnection(win.display);
  surface_create_info.window = xcb_generate_id(surface_create_info.connection);
  if(vkCreateXcbSurfaceKHR(vulkan_instance.instance, &surface_create_info,
			   NULL, &vulkan_surface) != VK_SUCCESS){
    printf("ERROR::unable to create XCB vulkan surface\n");
    return -1;
  }
#endif

	   
  VkPhysicalDevice physical_device = VK_NULL_HANDLE;
  
  if(nus_build_gpu_group(vulkan_instance.instance, &gpu_g) != NUS_SUCCESS){
    printf("ERROR::build gpu group returned NUS_FAILURE\n");
    return -1;
  }
  nus_check_gpu_presentation_support(vulkan_surface, &gpu_g);
  nus_print_gpu_group(gpu_g);
  //if(nus_check_gpu_presentation_support(vulkan_surface, &gpu_g) != NUS_SUCCESS){
  //printf("could not find support for presentation\n");
    return -1;
    //}
  
  /* gets queue of specific queue-family of specific device */
  /*vkGetDeviceQueue(vulkan_device, graphics_family, 0, &device_queue);*/

  
  while(run){
    nus_handle_system_events(win);
  }

  // free program
  /*
  if(VK_NULL_HANDLE != vulkan_device){
    vkDeviceWaitIdle(vulkan_device);
    vkDestroyDevice(vulkan_device, NULL);
  }
  */
  nus_free_vulkan_instance(&vulkan_instance);
  
  nus_free_window(&win);
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
void close_win(void){
  run = 0;
}
