#include <stdio.h>

#define NUS_SYSTEM_WINDOW
#include <NUS/NUS_library.h>

#define PROGRAM_NAME "unit_test-model_load"


int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}
  
  
  NUS_window win;
  if(nus_window_build(PROGRAM_NAME, 600, 400, &win) != NUS_SUCCESS){
    printf("ERROR::failed to create window\n");
    return -1;
  }

  nus_load_global_vulkan_library();
  
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
  
  NUS_vulkan_instance vulkan_instance;
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

  nus_select_gpu(multi_gpu);
  
  NUS_model model;
  
  if(nus_model_build(nus_absolute_path_build("butterfly.nusm"), &model) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to build model");
    return -1;
  }
  
  nus_model_free(&model);
  
  nus_multi_gpu_free(&multi_gpu);
  
  nus_vulkan_instance_free(&vulkan_instance);
  nus_free_vulkan_library();
  nus_window_free(&win);
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
