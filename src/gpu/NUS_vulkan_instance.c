#include "NUS_vulkan_instance.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static NUS_result nus_vulkan_instance_build_instance
(NUS_vulkan_instance *, NUS_string_group, NUS_string_group);
static NUS_result nus_vulkan_instance_extension_support(NUS_string_group);
static NUS_result nus_vulkan_instance_layer_support(NUS_string_group);

VkDebugReportCallbackEXT NUS_vulkan_debug_report_callback;

NUS_result nus_vulkan_instance_build
(NUS_vulkan_instance *p_NUS_vulkan_instance_,
 NUS_string_group extensions, NUS_string_group layers)
{
  if(nus_vulkan_instance_build_instance(p_NUS_vulkan_instance_,
					extensions, layers) != NUS_SUCCESS){
    printf("ERROR::failed to build vulkan instance instance\n");
    return NUS_FAILURE;
  }
  
  nus_load_instance_vulkan_library(p_NUS_vulkan_instance_->instance,
				   &p_NUS_vulkan_instance_->functions);
  nus_bind_instance_vulkan_library(p_NUS_vulkan_instance_->functions);

#if defined(NUS_DEBUG)
  
  // create debug callback
  
  VkDebugReportCallbackCreateInfoEXT callback_create_info = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
    .pNext = NULL,
    .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
    VK_DEBUG_REPORT_WARNING_BIT_EXT |
    VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
    .pfnCallback = &nus_vulkan_validation_callback,
    .pUserData = NULL
  };
  if(vkCreateDebugReportCallbackEXT(p_NUS_vulkan_instance_->instance,
				    &callback_create_info, NULL,
				    &p_NUS_vulkan_instance_->debug_report_callback) !=
     VK_SUCCESS){
    printf("ERROR::failed to create vulkan instance debug report\n");
    return NUS_FAILURE;
  }
  
#endif
  return NUS_SUCCESS;
}
void nus_vulkan_instance_free(NUS_vulkan_instance *p_NUS_vulkan_instance_)
{
#if defined(NUS_DEBUG)
  vkDestroyDebugReportCallbackEXT(p_NUS_vulkan_instance_->instance,
				  p_NUS_vulkan_instance_->debug_report_callback,
				  VK_NULL_HANDLE);
#endif
  if(VK_NULL_HANDLE != p_NUS_vulkan_instance_->instance){
    vkDestroyInstance(p_NUS_vulkan_instance_->instance, NULL);
  }
}
static NUS_result nus_vulkan_instance_build_instance
(NUS_vulkan_instance *p_NUS_vulkan_instance_,
 NUS_string_group extensions, NUS_string_group layers)
{
#if defined(NUS_DEBUG)
  NUS_string_group tmp_extensions,
    tmp_layers;
  nus_string_group_copy(&tmp_extensions, extensions);
  nus_string_group_copy(&tmp_layers, layers);
  nus_string_group_append(&tmp_extensions, "VK_EXT_debug_report");
  nus_string_group_append(&tmp_layers, "VK_LAYER_LUNARG_standard_validation");
  extensions = tmp_extensions;
  layers = tmp_layers;
#endif
  if(nus_vulkan_instance_extension_support(extensions) != NUS_SUCCESS){
    printf("ERROR::vulkan instance extension not supported\n");
    return NUS_FAILURE;
  }
  if(nus_vulkan_instance_layer_support(layers) != NUS_SUCCESS){
    printf("ERROR::vulkan instance layer not supported\n");
    return NUS_FAILURE;
  }
  VkApplicationInfo application_create_info = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = NULL,
    .pApplicationName = "application",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "NUS_engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_0
  };
  
  VkInstanceCreateInfo instance_create_info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .pApplicationInfo = &application_create_info,
    .enabledLayerCount = layers.count,
    .ppEnabledLayerNames = (char const * const *)layers.strings,
    .enabledExtensionCount = extensions.count,
    .ppEnabledExtensionNames = (char const * const *)extensions.strings
  };
  p_NUS_vulkan_instance_->instance = VK_NULL_HANDLE;
  if(vkCreateInstance(&instance_create_info, NULL,
		      &p_NUS_vulkan_instance_->instance) != VK_SUCCESS){
    printf("ERROR::unable to create vulkan instance\n");
    return NUS_FAILURE;
  }

#if defined(NUS_DEBUG)
  //free string groups we created earlier
  
  nus_string_group_free(&extensions);
  nus_string_group_free(&layers);
  
#endif
  return NUS_SUCCESS;
}
static NUS_result nus_vulkan_instance_extension_support
(NUS_string_group NUS_string_group_)
{
  unsigned int supported_extension_count,
    supported_wanted_extension_count,
    i;
  if(vkEnumerateInstanceExtensionProperties(NULL, &supported_extension_count,
					    NULL) != VK_SUCCESS ||
     supported_extension_count == 0){
    printf("ERROR::instance extension enumeration: count=%d\n",
	   supported_extension_count);
    return NUS_FAILURE;
  }
  VkExtensionProperties supported_extensions[supported_extension_count];
  if(vkEnumerateInstanceExtensionProperties(NULL, &supported_extension_count,
					    supported_extensions) != VK_SUCCESS){
    printf("ERROR::instance extension enumeration: extensions\n");
    return NUS_FAILURE;
  }

  // "count" tracks how many of the supported extensions are in the string group
  // we want this to be equal to the length of the string group
  for(i = 0, supported_wanted_extension_count = 0;
      i < supported_extension_count; ++i){
    if(nus_string_group_contains(NUS_string_group_,
				 supported_extensions[i].extensionName)){
      ++supported_wanted_extension_count;
    }
  }
  if(supported_wanted_extension_count < NUS_string_group_.count){
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_vulkan_instance_layer_support
(NUS_string_group NUS_string_group_)
{//TODO fix memleak originating from vkEnumereate...
  unsigned int supported_layer_count,
    supported_wanted_layer_count,
    i;
  
  if(vkEnumerateInstanceLayerProperties(&supported_layer_count,
					NULL) != VK_SUCCESS ||
     supported_layer_count == 0){
    printf("ERROR::instance layer enumeration: count=%d\n",
	   supported_layer_count);
    return NUS_FAILURE;
  }
  
  VkLayerProperties supported_layers[supported_layer_count];
  if(vkEnumerateInstanceLayerProperties(&supported_layer_count,
					supported_layers) != VK_SUCCESS){
    printf("ERROR::instance layer enumeration: layers\n");
    return NUS_FAILURE;
  }
  
  // "count" tracks how many of the supported layers are in the string group
  // we want this to be equal to the length of the string group
  
  for(i = 0, supported_wanted_layer_count = 0; i < supported_layer_count; ++i){
    if(nus_string_group_contains(NUS_string_group_,
				 supported_layers[i].layerName)){
      ++supported_wanted_layer_count;
    }
  }
  if(supported_wanted_layer_count < NUS_string_group_.count){
    return NUS_FAILURE;
  }
  
  return NUS_SUCCESS;
}
#if defined(NUS_DEBUG)

VKAPI_ATTR VkBool32 VKAPI_CALL nus_vulkan_validation_callback
(VkDebugReportFlagsEXT flags,
 VkDebugReportObjectTypeEXT objectType,
 uint64_t object,
 size_t location,
 int32_t messageCode,
 const char* pLayerPrefix,
 const char* pMessage,
 void* pUserData)
{
  printf("VALIDATION::ERROR:: %s\n", pMessage);
  return VK_FALSE;
}

#endif
