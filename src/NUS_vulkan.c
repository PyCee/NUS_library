#include "NUS_vulkan.h"
#include "NUS_engine.h"
#include "gpu/NUS_vulkan_instance.h"
#include "gpu/NUS_gpu_group.h"
#include <stdio.h>
#include <dlfcn.h>

#if defined(NUS_OS_WINDOWS)
#define NUS_load_proc_address GetProcAddress
static HWMODULE vulkan_library;
#elif defined(NUS_OS_UNIX)
#define NUS_load_proc_address dlsym
static void *vulkan_library;
#endif

#define NUS_VK_EXPORTED_FUNCTION(fun)					\
  do{									\
    if(!(fun = (PFN_##fun)NUS_load_proc_address( vulkan_library, #fun))){ \
      printf("ERROR::could not load export function %s\n", #fun);	\
    }									\
  } while(0)

#define NUS_VK_GLOBAL_LEVEL_FUNCTION(fun)					\
  do{									\
    if(!(fun = (PFN_##fun)vkGetInstanceProcAddr(NULL, #fun))){		\
      printf("ERROR::failed getting address to global level %s\n", #fun); \
    }									\
  } while(0)

/* macros for loading instance level function pointers */
#define NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, fun)		\
  do{									\
    if(!(functions->fun = (PFN_##fun)vkGetInstanceProcAddr(instance, #fun))){	\
      printf("ERROR::failed getting address to instance level %s\n", #fun); \
    }									\
  } while(0)
#if defined(NUS_OS_WINDOWS)
#define NUS_WINDOWS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, fun) \
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, fun)
#define NUS_UNIX_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, fun)
#elif defined(NUS_OS_UNIX)
#define NUS_WINDOWS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, fun)
#define NUS_UNIX_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, fun) \
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, fun)
#endif

/* macros for loading device level function pointers */
#define NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, fun)		\
  do{									\
    if(!(functions->fun = (PFN_##fun)vkGetDeviceProcAddr(device, #fun))){ \
      printf("ERROR::failed getting address to device level %s\n", #fun); \
    }									\
  } while(0)

#define NUS_SET_VK_FUNCTION(functions, fun)	\
  fun = functions.fun
#if defined(NUS_OS_WINDOWS)
#define NUS_WINDOWS_SET_VK_FUNCTION(functions, fun)	\
  NUS_SET_VK_FUNCTION(functions, fun)
#define NUS_UNIX_SET_VK_FUNCTION(functions, fun)
#elif defined(NUS_OS_UNIX)
#define NUS_WINDOWS_SET_VK_FUNCTION(functions, fun)
#define NUS_UNIX_SET_VK_FUNCTION(functions, fun)	\
  NUS_SET_VK_FUNCTION(functions, fun)
#endif

NUS_result nus_load_global_vulkan_library(void)
{
#if defined(NUS_OS_WINDOWS)
  vulkan_library = LoadLibrary("vulkan-1.dll");
#elif defined(NUS_OS_UNIX)
  vulkan_library = dlopen("libvulkan.so.1", RTLD_NOW);
#endif
  if(NULL == vulkan_library){
    printf("ERROR::failed to load vulkan_library\n");
    return NUS_FAILURE;
  }
  NUS_VK_EXPORTED_FUNCTION(vkGetInstanceProcAddr);
  NUS_VK_GLOBAL_LEVEL_FUNCTION(vkCreateInstance);
  NUS_VK_GLOBAL_LEVEL_FUNCTION(vkEnumerateInstanceExtensionProperties);
  NUS_VK_GLOBAL_LEVEL_FUNCTION(vkEnumerateInstanceLayerProperties);
  return NUS_SUCCESS;
}
NUS_result nus_free_vulkan_library(void)
{
#if defined(NUS_OS_WINDOWS)
  FreeLibrary(vulkan_library);
#elif defined(NUS_OS_UNIX)
  dlclose(vulkan_library);
#endif
  return NUS_SUCCESS;
}
void nus_load_instance_vulkan_library
(VkInstance instance, NUS_vk_instance_functions *functions)
{
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, vkGetDeviceProcAddr);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, vkDestroyInstance);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, vkEnumeratePhysicalDevices);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, vkGetPhysicalDeviceProperties);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, vkGetPhysicalDeviceFeatures);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions,
				 vkGetPhysicalDeviceQueueFamilyProperties);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions,
				 vkEnumerateDeviceExtensionProperties);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, vkCreateDevice);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions,
				 vkGetPhysicalDeviceSurfaceSupportKHR);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, vkDestroySurfaceKHR);
  NUS_WINDOWS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, vkCreateWin32SurfaceKHR);
  NUS_UNIX_VK_INSTANCE_LEVEL_FUNCTION(instance, functions, vkCreateXcbSurfaceKHR);
  NUS_VK_INSTANCE_LEVEL_FUNCTION(instance, functions,
			       vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
}
void nus_load_device_vulkan_library
(VkDevice device, NUS_vk_device_functions *functions)
{
  NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, vkDestroyDevice);
  NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, vkGetDeviceQueue);
  NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, vkDeviceWaitIdle);
  NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, vkCreateSwapchainKHR);
  NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, vkDestroySwapchainKHR);
  NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, vkGetSwapchainImagesKHR);
  NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, vkAcquireNextImageKHR);
  NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, vkQueuePresentKHR);
  NUS_VK_DEVICE_LEVEL_FUNCTION(device, functions, vkCreateSemaphore);
}
void nus_bind_instance_vulkan_library
(NUS_vk_instance_functions functions)
{
  NUS_SET_VK_FUNCTION(functions, vkGetDeviceProcAddr);			
  NUS_SET_VK_FUNCTION(functions, vkDestroyInstance);			
  NUS_SET_VK_FUNCTION(functions, vkEnumeratePhysicalDevices);		
  NUS_SET_VK_FUNCTION(functions, vkGetPhysicalDeviceProperties);		
  NUS_SET_VK_FUNCTION(functions, vkGetPhysicalDeviceFeatures);		
  NUS_SET_VK_FUNCTION(functions, vkGetPhysicalDeviceQueueFamilyProperties);
  NUS_SET_VK_FUNCTION(functions, vkEnumerateDeviceExtensionProperties);	
  NUS_SET_VK_FUNCTION(functions, vkCreateDevice);
  NUS_SET_VK_FUNCTION(functions, vkGetPhysicalDeviceSurfaceSupportKHR);
  NUS_SET_VK_FUNCTION(functions, vkDestroySurfaceKHR);
  NUS_WINDOWS_SET_VK_FUNCTION(functions, vkCreateWin32SurfaceKHR);		
  NUS_UNIX_SET_VK_FUNCTION(functions, vkCreateXcbSurfaceKHR);
  NUS_SET_VK_FUNCTION(functions, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
}
void nus_bind_device_vulkan_library
(NUS_vk_device_functions functions)
{
  NUS_SET_VK_FUNCTION(functions, vkDestroyDevice);
  NUS_SET_VK_FUNCTION(functions, vkGetDeviceQueue);
  NUS_SET_VK_FUNCTION(functions, vkDeviceWaitIdle);
  NUS_SET_VK_FUNCTION(functions, vkCreateSwapchainKHR);
  NUS_SET_VK_FUNCTION(functions, vkDestroySwapchainKHR);
  NUS_SET_VK_FUNCTION(functions, vkGetSwapchainImagesKHR);
  NUS_SET_VK_FUNCTION(functions, vkAcquireNextImageKHR);
  NUS_SET_VK_FUNCTION(functions, vkQueuePresentKHR);
  NUS_SET_VK_FUNCTION(functions, vkCreateSemaphore);
}
