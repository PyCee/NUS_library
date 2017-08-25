
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#define NUS_SYSTEM_RENDER
#include <NUS/NUS_library.h>
#include <limits.h>

#include <unistd.h>

#define PROGRAM_NAME "unit_test-uniform_buffer"

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
  nus_event_handler_append(eve, NUS_EVENT_CLOSE_WINDOW, 0, close_win);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_ESC, close_win);
  
  nus_event_handler_set(&eve);
  
  NUS_vulkan_instance vulkan_instance;
  
  NUS_string_group instance_extensions;
  nus_string_group_build(&instance_extensions);
  nus_string_group_append(&instance_extensions, 
			  VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(NUS_OS_WINDOWS)
  nus_string_group_append(&instance_extensions, 
			  VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(NUS_OS_UNIX)
  nus_string_group_append(&instance_extensions,
			  VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
  NUS_string_group instance_layers;
  nus_string_group_build(&instance_layers);
  
  if(nus_vulkan_instance_build(&vulkan_instance, instance_extensions,
			       instance_layers) != NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }
  nus_string_group_free(&instance_extensions);
  nus_string_group_free(&instance_layers);
  
  
  NUS_multi_gpu multi_gpu;
  if(nus_multi_gpu_build(vulkan_instance, &multi_gpu) != NUS_SUCCESS){
    printf("ERROR::build multi gpu returned NUS_FAILURE\n");
    return -1;
  }

  NUS_presentation_surface present;
  if(nus_presentation_surface_build(win, vulkan_instance, &multi_gpu, &present)
     != NUS_SUCCESS){
    printf("ERROR::failed to build presentaion surface\n");
    return -1;
  }
  
  // temp init code
  
  NUS_model model;
  // the vertex normal represents color for this unit test
  if(nus_model_build(nus_absolute_path_build("butterfly.nusm"), &model) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to build model\n");
    return -1;
  }

  for(unsigned int i = 0; i < model.animation_count; ++i){
    NUS_LOG("animation %d name: %s\n", i, model.animations[i].name);
  }

  
  run = 0;
  while(run){

    nus_system_events_handle(win);
    
    if(nus_multi_gpu_submit_commands(multi_gpu) != NUS_SUCCESS){
      printf("ERROR::failed to submit multi gpu command queues\n");
      return -1;
    }
    
    if(nus_presentation_surface_present(&present) != NUS_SUCCESS){
      printf("ERROR::failed to present window\n");
      return -1;
    }

    //run = 0;
    
  }
  
  printf("freeing unit test %s\n", PROGRAM_NAME);

  vkDeviceWaitIdle(nus_get_bound_device());
  
  nus_model_free(&model);
  
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
