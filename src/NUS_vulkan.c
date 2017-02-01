#include "NUS_vulkan.h"
#include "NUS_engine.h"
#include <stdio.h>

#include <dlfcn.h>

#if defined(NUS_OS_WINDOWS)
#define NUS_load_proc_address GetProcAddress
static HWMODULE vulkan_library;
#elif defined(NUS_OS_UNIX)
#define NUS_load_proc_address dlsym
static void *vulkan_library;
#endif

/* Macro for general loading vulkan function pointers */
#define NUS_VK_LOAD_FUNCTION_POINTER(fun, object, load_function)	\
  do{									\
    if(!(fun = (PFN_##fun)load_function( object, #fun))){		\
      printf("ERROR::could not load global function %s\n", #fun);	\
    }									\
  } while(0)


/* Macro for general loading vulkan function pointers */
#define NUS_VK_LOAD_LOCAL_FUNCTION_POINTER(fun, functions, object, load_function) \
  do{									\
    if(!(functions->fun = (PFN_##fun)load_function( object, #fun))){	\
      printf("ERROR::could not load global function %s\n", #fun);	\
    }									\
  } while(0)

/* Macros for loading instance level function pointer */
#define NUS_VK_LOAD_INSTANCE_FUNCTION_POINTER(fun, functions, instance) \
  NUS_VK_LOAD_LOCAL_FUNCTION_POINTER(fun, functions, instance, vkGetInstanceProcAddr)
/* Macros for loading device level function pointer */
#define NUS_VK_LOAD_DEVICE_FUNCTION_POINTER(fun, functions, device) \
  NUS_VK_LOAD_LOCAL_FUNCTION_POINTER(fun, functions, device, vkGetDeviceProcAddr)

#define NUS_VK_SET_FUNCTION_POINTER(fun, functions)	\
  fun = functions.fun

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
  NUS_VK_UNIVERSAL_FUNCTION(NUS_VK_LOAD_FUNCTION_POINTER, vulkan_library,
			    NUS_load_proc_address);
  NUS_VK_GLOBAL_FUNCTION(NUS_VK_LOAD_FUNCTION_POINTER, NULL,
			 vkGetInstanceProcAddr);
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
  NUS_VK_INSTANCE_FUNCTION(NUS_VK_LOAD_INSTANCE_FUNCTION_POINTER, functions, instance);
}
void nus_load_device_vulkan_library
(VkDevice device, NUS_vk_device_functions *functions)
{
  NUS_VK_DEVICE_FUNCTION(NUS_VK_LOAD_DEVICE_FUNCTION_POINTER, functions, device);
}
void nus_bind_instance_vulkan_library
(NUS_vk_instance_functions functions)
{
  NUS_VK_INSTANCE_FUNCTION(NUS_VK_SET_FUNCTION_POINTER, functions);
}
void nus_bind_device_vulkan_library
(NUS_vk_device_functions functions)
{
  NUS_VK_DEVICE_FUNCTION(NUS_VK_SET_FUNCTION_POINTER, functions);
}
