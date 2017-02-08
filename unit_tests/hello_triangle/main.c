
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#define NUS_SYSTEM_RENDER
#include <NUS/NUS_library.h>
#include <limits.h>

#include <unistd.h>

#define PROGRAM_NAME "unit_test-hello_triangle"

void close_win(void);
char run;

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  nus_load_global_vulkan_library();
  
  NUS_window win;
  if(nus_window_build(PROGRAM_NAME, 600, 400, &win) != NUS_SUCCESS){
    printf("ERROR::failed to create window\n");
    return -1;
  }
  NUS_event_handler eve;
  if(nus_event_handler_build(&eve) != NUS_SUCCESS){
    printf("ERROR::failed to build event handler\n");
    return -1;
  }
  nus_event_handler_function_append(eve, NUS_EVENT_CLOSE_WINDOW, 0, close_win);
  
  nus_event_handler_function_append(eve, NUS_EVENT_KEY_PRESS,
				    NUS_KEY_ESC, close_win);
  nus_event_handler_set(&eve);
  
  NUS_vulkan_instance vulkan_instance;
  char *extensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(NUS_OS_WINDOWS)
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(NUS_OS_UNIX)
    VK_KHR_XCB_SURFACE_EXTENSION_NAME
#endif
  };
  nus_vulkan_instance_set_extensions(2, extensions, &vulkan_instance);
  
  if(nus_vulkan_instance_build(&vulkan_instance)
     != NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }
  
  NUS_multi_gpu multi_gpu;
  if(nus_multi_gpu_build(vulkan_instance.instance, &multi_gpu) != NUS_SUCCESS){
    printf("ERROR::build multi gpu returned NUS_FAILURE\n");
    return -1;
  }
  
  NUS_presentation_surface present;
  if(nus_presentation_surface_build(win, vulkan_instance, &multi_gpu, &present)
     != NUS_SUCCESS){
    printf("ERROR::failed to build presentaion surface\n");
    return -1;
  }

  
  run = 1;
  while(run){
    nus_system_events_handle(win);
    
    
    //TODO: fix
    //  if I change the clear color at runtime, the window flickers black
    //    this only happens If i updateat a certain speed
    
    if(nus_image_clear(present.image_available,
		       present.image_rendered,
		       (VkClearColorValue){{0.0f, 0.0f, 0.0f, 0.0f}},
		       multi_gpu.gpus[0], present.render_image) !=
       NUS_SUCCESS){
      printf("ERROR::failed to clear window\n");
      return -1;
    }
    if(nus_multi_gpu_submit_commands(multi_gpu) != NUS_SUCCESS){
      printf("ERROR::failed to submit multi gpu command queues\n");
      return -1;
    }
    if(nus_presentation_surface_present(&present) != NUS_SUCCESS){
      printf("ERROR::failed to present window\n");
      return -1;
    }
  }
  
  printf("freeing unit test %s\n", PROGRAM_NAME);
   
  nus_presentation_surface_free(vulkan_instance, &present);
  nus_multi_gpu_free(&multi_gpu);
  nus_vulkan_instance_free(&vulkan_instance);
  nus_free_vulkan_library();
  
  nus_window_free(&win);
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
void close_win(void)
{
  run = 0;
}
