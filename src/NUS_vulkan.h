#ifndef _NUS_VULKAN_H_
#define _NUS_VULKAN_H_

#include "NUS_result.h"
#include "NUS_os.h"

#define VK_NO_PROTOTYPES

#if defined(NUS_OS_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(NUS_OS_UNIX)
#define VK_USE_PLATFORM_XCB_KHR
#endif

#include <vulkan/vulkan.h>

#define NUS_VK_FUNCTION_DECLARATION(fun)	\
  PFN_##fun fun

/* Will declare vulkan function pointer only if compiled on windows */
#if defined(NUS_OS_WINDOWS)
#define NUS_WINDOWS_VK_FUNCTION_DECLARATION(fun)	\
  NUS_VK_FUNCTION_DECLARATION(fun)
#define NUS_UNIX_VK_FUNCTION_DECLARATION
/* Will declare vulkan function pointer only if compiled on unix */
#elif defined(NUS_OS_UNIX)
#define NUS_WINDOWS_VK_FUNCTION_DECLARATION(fun)
#define NUS_UNIX_VK_FUNCTION_DECLARATION(fun)	\
  NUS_VK_FUNCTION_DECLARATION(fun)
#endif

/* Declarations for global level vulkan functions */
#define NUS_DECLARE_GLOBAL_VK_FUNCTIONS					\
  NUS_VK_FUNCTION_DECLARATION(vkGetInstanceProcAddr);			\
  NUS_VK_FUNCTION_DECLARATION(vkCreateInstance);			\
  NUS_VK_FUNCTION_DECLARATION(vkEnumerateInstanceExtensionProperties);	\
  NUS_VK_FUNCTION_DECLARATION(vkEnumerateInstanceLayerProperties)

/* Declarations for instance level vulkan functions */
#define NUS_DECLARE_INSTANCE_VK_FUNCTIONS				\
  NUS_VK_FUNCTION_DECLARATION(vkGetDeviceProcAddr);			\
  NUS_VK_FUNCTION_DECLARATION(vkDestroyInstance);			\
  NUS_VK_FUNCTION_DECLARATION(vkEnumeratePhysicalDevices);		\
  NUS_VK_FUNCTION_DECLARATION(vkGetPhysicalDeviceProperties);		\
  NUS_VK_FUNCTION_DECLARATION(vkGetPhysicalDeviceFeatures);		\
  NUS_VK_FUNCTION_DECLARATION(vkGetPhysicalDeviceQueueFamilyProperties); \
  NUS_VK_FUNCTION_DECLARATION(vkEnumerateDeviceExtensionProperties);	\
  NUS_VK_FUNCTION_DECLARATION(vkCreateDevice);				\
  NUS_VK_FUNCTION_DECLARATION(vkGetPhysicalDeviceSurfaceSupportKHR);	\
  NUS_VK_FUNCTION_DECLARATION(vkDestroySurfaceKHR);			\
  NUS_WINDOWS_VK_FUNCTION_DECLARATION(vkCreateWin32SurfaceKHR);		\
  NUS_UNIX_VK_FUNCTION_DECLARATION(vkCreateXcbSurfaceKHR);		

/* Declarations for device level vulkan functions */
#define NUS_DECLARE_DEVICE_VK_FUNCTIONS			\
  NUS_UNIX_VK_FUNCTION_DECLARATION(vkDestroyDevice);	\
  NUS_UNIX_VK_FUNCTION_DECLARATION(vkGetDeviceQueue);	\
  NUS_UNIX_VK_FUNCTION_DECLARATION(vkCreateSwapchainKHR);	\
  NUS_UNIX_VK_FUNCTION_DECLARATION(vkDestroySwapchainKHR);	\
  NUS_UNIX_VK_FUNCTION_DECLARATION(vkGetSwapchainImagesKHR);	\
  NUS_UNIX_VK_FUNCTION_DECLARATION(vkAcquireNextImageKHR);	\
  NUS_UNIX_VK_FUNCTION_DECLARATION(vkQueuePresentKHR);	
    
NUS_DECLARE_GLOBAL_VK_FUNCTIONS;
NUS_DECLARE_INSTANCE_VK_FUNCTIONS;
NUS_DECLARE_DEVICE_VK_FUNCTIONS;

typedef struct NUS_vk_instance_functions{
  NUS_DECLARE_INSTANCE_VK_FUNCTIONS;
} NUS_vk_instance_functions;

typedef struct NUS_vk_device_functions{
  NUS_DECLARE_DEVICE_VK_FUNCTIONS;
} NUS_vk_device_functions;


struct NUS_vulkan_instance;
struct NUS_gpu;
NUS_result nus_load_global_vulkan_library(void);
void nus_load_instance_vulkan_library(VkInstance, NUS_vk_instance_functions *);
void nus_load_device_vulkan_library(VkDevice, NUS_vk_device_functions *);
void nus_bind_instance_vulkan_library(NUS_vk_instance_functions);
void nus_bind_device_vulkan_library(NUS_vk_device_functions);

#endif /* _NUS_VULKAN_H_ */
