
#include <stdio.h>
#include <string.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#define NUS_SYSTEM_RENDER
#include <NUS/NUS_engine.h>
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

  
  NUS_window win = nus_build_window(PROGRAM_NAME, 600, 400);
  nus_setup_system_events(win);
  NUS_event_handler eve = nus_build_event_handler();
  eve.close_window = close_win;
  nus_set_event_handler(&eve);
  

  nus_load_global_vulkan_library();
  
  NUS_vulkan_instance vulkan_instance;
  nus_init_vulkan_instance(&vulkan_instance);
  
  nus_vulkan_instance_add_extension(VK_KHR_SURFACE_EXTENSION_NAME,
				    &vulkan_instance);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  nus_vulkan_instance_add_extension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
				    &vulkan_instance);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  nus_vulkan_instance_add_extension(VK_KHR_XCB_SURFACE_EXTENSION_NAME,
				    &vulkan_instance);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
  nus_vulkan_instance_add_extension(VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
				    &vulkan_instance);
#endif
  
  if(nus_build_vulkan_instance(&vulkan_instance)
     != NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }
  
  NUS_presentation_surface present;
  if(nus_build_presentation_surface(win, vulkan_instance, &present)
     != NUS_SUCCESS){
    printf("ERROR::failed to build presentaion surface\n");
    return -1;
  }
  NUS_gpu_group gpu_g;
 
  if(nus_gpu_group_build(vulkan_instance.instance, &gpu_g) != NUS_SUCCESS){
    printf("ERROR::build gpu group returned NUS_FAILURE\n");
    return -1;
  }
  nus_gpu_group_check_surface_support(present.surface, &gpu_g);
  nus_gpu_group_print(gpu_g);

  //tmp code
  
  nus_bind_device_vulkan_library(gpu_g.gpus[0].functions);
  
  VkSemaphore image_available,
    render_finished;
  VkSemaphoreCreateInfo semaphore_create_info;
  semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphore_create_info.pNext = NULL;
  semaphore_create_info.flags = 0;

  printf("prior to semaphore creation\n");
  if((vkCreateSemaphore(gpu_g.gpus[0].logical_device, &semaphore_create_info, NULL,
			&image_available) != VK_SUCCESS) ||
     (vkCreateSemaphore(gpu_g.gpus[0].logical_device, &semaphore_create_info, NULL,
		       &render_finished) != VK_SUCCESS)){
    printf("ERROR::failed to create semaphores\n");
    return -1;
  }
  printf("after semaphore creation\n");
  VkSurfaceCapabilitiesKHR surface_capabilities;
  if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu_g.physical_devices[0],
					    present.surface, &surface_capabilities)
     != VK_SUCCESS){
    printf("ERROR::failed to get surface capabilities\n");
    return -1;
  }
  printf("after get surface capabilities\n");
  //end of tmp code
  
  //run = 1;
  run = 0;
  while(run){
    nus_handle_system_events(win);
    //clear window to color
  }
  
  nus_gpu_group_free(&gpu_g); 
  nus_free_presentation_surface(vulkan_instance, &present);
  
  printf("freeing unit test %s\n", PROGRAM_NAME);
  nus_free_vulkan_instance(&vulkan_instance);
  nus_free_window(&win);
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
void close_win(void){
  run = 0;
}
