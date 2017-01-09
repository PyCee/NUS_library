
#include <stdio.h>
#include <string.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#define NUS_SYSTEM_RENDER
#include <NUS/NUS_engine.h>
#include <limits.h>

#define PROGRAM_NAME "unit_test-gpu_detection"

#include <X11/Xlib-xcb.h>

char run;

void close_win(void);

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  NUS_window win = nus_build_window(PROGRAM_NAME, 600, 400);
  
  NUS_gpu_group gpu_g;

  NUS_vulkan_instance_info instance_info;
  if(nus_build_vulkan_instance_info(&instance_info)
     != NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }

  nus_vulkan_instance_info_add_extension(VK_KHR_SURFACE_EXTENSION_NAME,
					 &instance_info);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  nus_vulkan_instance_info_add_extension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
					 &instance_info);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  nus_vulkan_instance_info_add_extension(VK_KHR_XCB_SURFACE_EXTENSION_NAME,
					 &instance_info);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
  nus_vulkan_instance_info_add_extension(VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
					 &instance_info);
#endif

  NUS_vulkan_instance vulkan_instance;
  if(nus_build_vulkan_instance(instance_info, &vulkan_instance)
     != NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }
  nus_free_vulkan_instance_info(&instance_info);
  
  NUS_presentation_surface present;
  if(nus_build_presentation_surface(win, vulkan_instance, &present)
     != NUS_SUCCESS){
    printf("ERROR::failed to build presentaion surface\n");
    return -1;
  }
  /*
  VkPhysicalDevice physical_device = VK_NULL_HANDLE;
  
  if(nus_build_gpu_group(vulkan_instance.instance, &gpu_g) != NUS_SUCCESS){
    printf("ERROR::build gpu group returned NUS_FAILURE\n");
    return -1;
  }
  nus_check_gpu_presentation_support(vulkan_surface, &gpu_g);
  nus_print_gpu_group(gpu_g);

  nus_free_vulkan_instance(&vulkan_instance);
  nus_free_window(&win);*/
  
  VkPhysicalDevice a_gpu;
  uint32_t present_queue_index;

  uint32_t num_gpus = 0;
  vkEnumeratePhysicalDevices(vulkan_instance.instance, &num_gpus, NULL);
  VkPhysicalDevice gpus[num_gpus];
  vkEnumeratePhysicalDevices(vulkan_instance.instance, &num_gpus, gpus);
  uint8_t found = 0;
  for(uint32_t i = 0; i < num_gpus; i++) {
    VkPhysicalDeviceProperties device_props;
    vkGetPhysicalDeviceProperties(gpus[i], &device_props);
    uint32_t qfc = 0; // queue family count
    vkGetPhysicalDeviceQueueFamilyProperties(gpus[i], &qfc, NULL);
    VkQueueFamilyProperties qfp[qfc]; // qf properties
    vkGetPhysicalDeviceQueueFamilyProperties(gpus[i], &qfc, qfp);
    for(uint32_t j = 0; j < qfc; j++) {
      VkBool32 supportsPresent;
      vkGetPhysicalDeviceSurfaceSupportKHR(gpus[i], j,
					   present.surface, &supportsPresent);
      printf("SUPORROOT %d\n", supportsPresent);
      
      if(supportsPresent != 0 &&
	 (qfp[j].queueFlags &VK_QUEUE_GRAPHICS_BIT) != 0){
        a_gpu = gpus[i];
        present_queue_index = j;
	found = 1;
	break;
      }
    }
  }
  if(found == 0) {
    puts("Can't find a physical device that can render & present!");
    return -1;
  }
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
