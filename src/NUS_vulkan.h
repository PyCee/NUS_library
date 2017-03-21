#ifndef NUS_VULKAN_H
#define NUS_VULKAN_H

#include "NUS_result.h"
#include "NUS_os.h"

#define VK_NO_PROTOTYPES

#if defined(NUS_OS_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(NUS_OS_UNIX)
#define VK_USE_PLATFORM_XCB_KHR
#endif
#include <vulkan/vulkan.h>

/* Call function only if compiled on windows */
#if defined(NUS_OS_WINDOWS)
#define NUS_VK_WINDOWS_FUNCTION(fun, function, ...) \
  function( fun, ##__VA_ARGS__)
#define NUS_VK_UNIX_FUNCTION(fun, function, ...)
/* Call function only if compiled on unix */
#elif defined(NUS_OS_UNIX)
#define NUS_VK_WINDOWS_FUNCTION(fun, function, ...)
#define NUS_VK_UNIX_FUNCTION(fun, function, ...) \
  function( fun, ##__VA_ARGS__)
#endif

/* Macro to call any function on all universal vk function pointers */
#define NUS_VK_UNIVERSAL_FUNCTION(function, ...)			\
  function( vkGetInstanceProcAddr, ##__VA_ARGS__ );			\
  
/* Macro to call any function on all global vk function pointers */
#define NUS_VK_GLOBAL_FUNCTION(function, ...)				\
  function( vkCreateInstance, ##__VA_ARGS__ );				\
  function( vkEnumerateInstanceExtensionProperties, ##__VA_ARGS__ );	\
  function( vkEnumerateInstanceLayerProperties, ##__VA_ARGS__ )


/* Macro to call any function on all validation instance vk function pointers */
#if defined(NUS_DEBUG)
//#define NUS_VK_VALIDATION_FUNCTION(function, ...)

#define NUS_VK_VALIDATION_FUNCTION(function, ...)		       \
  function( vkCreateDebugReportCallbackEXT, ##__VA_ARGS__ );	       \
  function( vkDebugReportMessageEXT, ##__VA_ARGS__ );		       \
  function( vkDestroyDebugReportCallbackEXT, ##__VA_ARGS__ )
#else
#define NUS_VK_VALIDATION_FUNCTION(function, ...)
#endif

/* Macro to call any function on all instance vk function pointers */
#define NUS_VK_INSTANCE_FUNCTION(function, ...)				\
  function( vkGetDeviceProcAddr, ##__VA_ARGS__ );			\
  function( vkDestroyInstance, ##__VA_ARGS__ );				\
  function( vkEnumeratePhysicalDevices, ##__VA_ARGS__ );		\
  function( vkGetPhysicalDeviceProperties, ##__VA_ARGS__ );		\
  function( vkGetPhysicalDeviceFeatures, ##__VA_ARGS__ );		\
  function( vkGetPhysicalDeviceQueueFamilyProperties, ##__VA_ARGS__ );	\
  function( vkEnumerateDeviceExtensionProperties, ##__VA_ARGS__ );	\
  function( vkCreateDevice, ##__VA_ARGS__ );				\
  function( vkGetPhysicalDeviceSurfaceSupportKHR, ##__VA_ARGS__ );	\
  function( vkDestroySurfaceKHR, ##__VA_ARGS__ );			\
  NUS_VK_WINDOWS_FUNCTION(vkCreateWin32SurfaceKHR, function ##__VA_ARGS__ ); \
  NUS_VK_UNIX_FUNCTION(vkCreateXcbSurfaceKHR, function, ##__VA_ARGS__ ); \
  function( vkGetPhysicalDeviceSurfaceCapabilitiesKHR, ##__VA_ARGS__ ); \
  function( vkGetPhysicalDeviceSurfaceFormatsKHR, ##__VA_ARGS__ );	\
  function( vkGetPhysicalDeviceSurfacePresentModesKHR, ##__VA_ARGS__ );	\
  NUS_VK_VALIDATION_FUNCTION( function, ##__VA_ARGS__ )


/* Macro to call any function on all device vk function pointers */
#define NUS_VK_DEVICE_FUNCTION(function, ...)				\
  function( vkDestroyDevice, ##__VA_ARGS__ );				\
  function( vkGetDeviceQueue, ##__VA_ARGS__ );				\
  function( vkDeviceWaitIdle, ##__VA_ARGS__ );				\
  function( vkCreateSwapchainKHR, ##__VA_ARGS__ );			\
  function( vkDestroySwapchainKHR, ##__VA_ARGS__ );			\
  function( vkGetSwapchainImagesKHR, ##__VA_ARGS__ );			\
  function( vkAcquireNextImageKHR, ##__VA_ARGS__ );			\
  function( vkQueuePresentKHR, ##__VA_ARGS__ );				\
  function( vkCreateSemaphore, ##__VA_ARGS__ );				\
  function( vkCreateCommandPool, ##__VA_ARGS__ );			\
  function( vkAllocateCommandBuffers, ##__VA_ARGS__ );			\
  function( vkBeginCommandBuffer, ##__VA_ARGS__ );			\
  function( vkCmdPipelineBarrier, ##__VA_ARGS__ );			\
  function( vkCmdClearColorImage, ##__VA_ARGS__ );			\
  function( vkEndCommandBuffer, ##__VA_ARGS__ );			\
  function( vkQueueSubmit, ##__VA_ARGS__ );				\
  function( vkDestroySemaphore, ##__VA_ARGS__ );			\
  function( vkCreateFence, ##__VA_ARGS__ );				\
  function( vkFreeCommandBuffers, ##__VA_ARGS__ );			\
  function( vkDestroyCommandPool, ##__VA_ARGS__ );			\
  function( vkCreateShaderModule, ##__VA_ARGS__ );			\
  function( vkCreateRenderPass, ##__VA_ARGS__ );			\
  function( vkCreateImageView, ##__VA_ARGS__ );				\
  function( vkCreateFramebuffer, ##__VA_ARGS__ );			\
  function( vkCreatePipelineLayout, ##__VA_ARGS__ );			\
  function( vkCreateGraphicsPipelines, ##__VA_ARGS__ );			\
  function( vkCmdBeginRenderPass, ##__VA_ARGS__ );			\
  function( vkCmdBindPipeline, ##__VA_ARGS__ );				\
  function( vkCmdDraw, ##__VA_ARGS__ );					\
  function( vkCmdEndRenderPass, ##__VA_ARGS__ );			\
  function( vkWaitForFences, ##__VA_ARGS__ );				\
  function( vkDestroyFence, ##__VA_ARGS__ );				\
  function( vkResetCommandPool, ##__VA_ARGS__ );			\
  function( vkResetFences, ##__VA_ARGS__ );				\
  function( vkDestroyPipeline, ##__VA_ARGS__ );				\
  function( vkDestroyRenderPass, ##__VA_ARGS__ );			\
  function( vkDestroyFramebuffer, ##__VA_ARGS__ );			\
  function( vkDestroyImageView, ##__VA_ARGS__ );			\
  function( vkDestroyPipelineLayout, ##__VA_ARGS__ );			\
  function( vkDestroyShaderModule, ##__VA_ARGS__ );			\
  function( vkCreateImage, ##__VA_ARGS__ );				\
  function( vkCmdCopyImage, ##__VA_ARGS__ );				\
  function( vkDestroyImage, ##__VA_ARGS__ );				\
  function( vkCreateBuffer, ##__VA_ARGS__ );				\
  function( vkGetBufferMemoryRequirements, ##__VA_ARGS__ );		\
  function( vkGetPhysicalDeviceMemoryProperties, ##__VA_ARGS__ );	\
  function( vkAllocateMemory, ##__VA_ARGS__ );				\
  function( vkBindBufferMemory, ##__VA_ARGS__ );			\
  function( vkMapMemory, ##__VA_ARGS__ );				\
  function( vkFlushMappedMemoryRanges, ##__VA_ARGS__ );			\
  function( vkUnmapMemory, ##__VA_ARGS__ );				\
  function( vkGetImageMemoryRequirements, ##__VA_ARGS__ );		\
  function( vkBindImageMemory, ##__VA_ARGS__ );				\
  function( vkFreeMemory, ##__VA_ARGS__ );				\
  function( vkCmdSetViewport, ##__VA_ARGS__ );				\
  function( vkCmdSetScissor, ##__VA_ARGS__ );				\
  function( vkCmdBindVertexBuffers, ##__VA_ARGS__ );			\
  function( vkDestroyBuffer, ##__VA_ARGS__ );				\
  function( vkCmdBindIndexBuffer, ##__VA_ARGS__ );			\
  function( vkCmdDrawIndexed, ##__VA_ARGS__ );				\
  function( vkGetFenceStatus, ##__VA_ARGS__ )


#define NUS_VK_FUNCTION_DECLARATION(fun)	\
  PFN_##fun fun

NUS_VK_UNIVERSAL_FUNCTION( NUS_VK_FUNCTION_DECLARATION );
NUS_VK_GLOBAL_FUNCTION( NUS_VK_FUNCTION_DECLARATION );
NUS_VK_INSTANCE_FUNCTION( NUS_VK_FUNCTION_DECLARATION );
NUS_VK_DEVICE_FUNCTION( NUS_VK_FUNCTION_DECLARATION );

typedef struct NUS_vk_instance_functions{
  NUS_VK_INSTANCE_FUNCTION( NUS_VK_FUNCTION_DECLARATION );
} NUS_vk_instance_functions;

typedef struct NUS_vk_device_functions{
  NUS_VK_DEVICE_FUNCTION( NUS_VK_FUNCTION_DECLARATION );
} NUS_vk_device_functions;

struct NUS_vulkan_instance;
struct NUS_gpu;
NUS_result nus_load_global_vulkan_library(void);
NUS_result nus_free_vulkan_library(void);
void nus_load_instance_vulkan_library(VkInstance, NUS_vk_instance_functions *);
void nus_load_device_vulkan_library(VkDevice, NUS_vk_device_functions *);
void nus_bind_instance_vulkan_library(NUS_vk_instance_functions);
void nus_bind_device_vulkan_library(NUS_vk_device_functions);

#endif /* NUS_VULKAN_H */
