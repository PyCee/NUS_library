
#include <stdio.h>
#include <string.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#define NUS_SYSTEM_RENDER
#include <NUS/NUS_library.h>
#include <limits.h>

#define PROGRAM_NAME "unit_test-gpu_detection"


char run;
void close_win(void);

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
  
  nus_vulkan_instance_set_layers(0, NULL, &vulkan_instance);
  
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
  
  nus_multi_gpu_print(multi_gpu);
  
  printf("freeing unit test %s\n", PROGRAM_NAME);
  
  nus_presentation_surface_free(vulkan_instance, &present);
  
  nus_multi_gpu_free(&multi_gpu);
  
  nus_vulkan_instance_free(&vulkan_instance);
  nus_free_vulkan_library();
  
  nus_window_free(&win);
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
