#include "NUS_vulkan_instance.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static NUS_result nus_vulkan_instance_check_support(NUS_vulkan_instance);
static NUS_result nus_check_extension_support(char **, unsigned int);
static NUS_result nus_check_layer_support(char **, unsigned int);

#if defined(NUS_DEBUG)
static VKAPI_ATTR VkBool32 VKAPI_CALL nus_validation_debug_callback
(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t,
 const char *, const char *, void *);
static VkDebugReportCallbackEXT debug_callback;
#endif

NUS_result nus_vulkan_instance_build
(NUS_vulkan_instance *NUS_vulkan_instance_)
{
  VkApplicationInfo application_create_info;
  VkInstanceCreateInfo instance_create_info;

  const char * const *extensions =
    (const char * const *)NUS_vulkan_instance_->extensions;
  const char * const *layers =
    (const char * const *)NUS_vulkan_instance_->layers;
  
  if(nus_vulkan_instance_check_support(*NUS_vulkan_instance_) != NUS_SUCCESS){
    printf("ERROR::vulkan instance info not supported\n");
    return NUS_FAILURE;
  }
  application_create_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  application_create_info.pNext = NULL;
  application_create_info.pApplicationName = "application";
  application_create_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  application_create_info.pEngineName = "NUS_engine";
  application_create_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  application_create_info.apiVersion = VK_API_VERSION_1_0; 
  instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instance_create_info.pNext = NULL;
  instance_create_info.flags = 0;
  instance_create_info.pApplicationInfo = &application_create_info;
  instance_create_info.enabledLayerCount = NUS_vulkan_instance_->layer_count;
  instance_create_info.ppEnabledLayerNames = layers;
  instance_create_info.enabledExtensionCount = NUS_vulkan_instance_->extension_count;
  instance_create_info.ppEnabledExtensionNames = extensions;
  if(vkCreateInstance(&instance_create_info, NULL, &NUS_vulkan_instance_->instance)
     != VK_SUCCESS){
    printf("ERROR::unable to create vulkan instance\n");
    return NUS_FAILURE;
  }
  
  nus_load_instance_vulkan_library(NUS_vulkan_instance_->instance,
				   &NUS_vulkan_instance_->functions);
  nus_bind_instance_vulkan_library(NUS_vulkan_instance_->functions);

#if defined(NUS_DEBUG)
  VkDebugReportCallbackCreateInfoEXT callback_create_info = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
    .pNext = NULL,
    .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
    VK_DEBUG_REPORT_WARNING_BIT_EXT |
    VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
    .pfnCallback = &nus_validation_debug_callback,
    .pUserData = NULL
  };
  if(vkCreateDebugReportCallbackEXT(NUS_vulkan_instance_->instance,
				    &callback_create_info, NULL,
				    &debug_callback) != VK_SUCCESS){
    printf("ERROR::failed to create vulkan instance debug report\n");
    return NUS_FAILURE;
  }
#endif

  
  return NUS_SUCCESS;
}
void nus_vulkan_instance_free(NUS_vulkan_instance *NUS_vulkan_instance_)
{
  unsigned char i;

#if defined(NUS_DEBUG)
  vkDestroyDebugReportCallbackEXT(NUS_vulkan_instance_->instance,
				  debug_callback, VK_NULL_HANDLE);
#endif
  if(VK_NULL_HANDLE != NUS_vulkan_instance_->instance){
    vkDestroyInstance(NUS_vulkan_instance_->instance, NULL);
  }
  if(NUS_vulkan_instance_->extensions){
    for(i = 0; i < NUS_vulkan_instance_->extension_count; ++i){
      free(NUS_vulkan_instance_->extensions[i]);
    }
    free(NUS_vulkan_instance_->extensions);
    NUS_vulkan_instance_->extensions = NULL;
    NUS_vulkan_instance_->extension_count = 0;
  }
  if(NUS_vulkan_instance_->layers){
    for(i = 0; i < NUS_vulkan_instance_->layer_count; ++i){
      free(NUS_vulkan_instance_->layers[i]);
    }
    free(NUS_vulkan_instance_->layers);
    NUS_vulkan_instance_->layers = NULL;
    NUS_vulkan_instance_->layer_count = 0;
  }
}

static NUS_result nus_vulkan_instance_check_support
(NUS_vulkan_instance NUS_vulkan_instance_)
{
  if(nus_check_extension_support(NUS_vulkan_instance_.extensions,
				 NUS_vulkan_instance_.extension_count) !=
     NUS_SUCCESS){
    printf("ERROR::failed to check support for extensions\n");
    return NUS_FAILURE;
  } 
  if(nus_check_layer_support(NUS_vulkan_instance_.layers,
			     NUS_vulkan_instance_.layer_count) != NUS_SUCCESS){
    printf("ERROR::failed to check support for layers\n");
  return NUS_FAILURE;
}
  return NUS_SUCCESS;
}
static NUS_result nus_check_extension_support
(char **extensions, unsigned int extension_count)
{
  if(0 < extension_count){
    unsigned int supported_extension_count;
    unsigned char i,
      j;
    supported_extension_count = 0;
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
    for(i = 0; i < extension_count; ++i){
      for(j = 0; j < supported_extension_count; ++j){
	if(strcmp(extensions[i], supported_extensions[j].extensionName) == 0){
	  break;
	}
	if(supported_extension_count - 1 == j){
	  printf("ERROR::attempting to use unsupported extension %s\n",
		 extensions[i]);
	  return NUS_FAILURE;
	}
      }
    }
  }
  return NUS_SUCCESS;
}
static NUS_result nus_check_layer_support
(char **layers, unsigned int layer_count)
{
  if(0 < layer_count){
    unsigned int supported_layer_count;
    unsigned char i,
      j;
    supported_layer_count = 0;
    if(vkEnumerateInstanceLayerProperties(&supported_layer_count,
					  NULL) != VK_SUCCESS ||
       supported_layer_count == 0){
      printf("ERROR::instance layer enumeration: count=%d\n", supported_layer_count);
      return NUS_FAILURE;
    }
    VkLayerProperties supported_layers[supported_layer_count];
    if(vkEnumerateInstanceLayerProperties(&supported_layer_count,
					  supported_layers) != VK_SUCCESS){
      printf("ERROR::instance layer enumeration: layers\n");
      return NUS_FAILURE;
    }
    for(i = 0; i < layer_count; ++i){
      for(j = 0; j < supported_layer_count; ++j){
	if(strcmp(layers[i], supported_layers[j].layerName) == 0){
	  break;
	}
	if(supported_layer_count - 1 == j){
	printf("ERROR::attempting to use unsupported layer %s\n",
	       layers[i]);
	return NUS_FAILURE;
	}
      }
    }
  }
  return NUS_SUCCESS;
}

#define NUS_VULKAN_INSTANCE_ADD_INFO(p_instance, info, count, i)	\
  do{									\
    p_instance->info##_count = count;					\
    p_instance->info##s = malloc(sizeof(*p_instance->info##s) * count);	\
    for(i = 0; i < count; ++i){						\
      p_instance->info##s[i] = malloc(sizeof(**p_instance->info##s) *	\
				      (strlen(info[i]) + 1));		\
      strcpy(p_instance->info##s[i], info[i]);				\
    }									\
  }while(0)

NUS_result nus_vulkan_instance_set_extensions
(unsigned int extension_count, char **extension_param,
 NUS_vulkan_instance *NUS_vulkan_instance_)
{
  unsigned char i;
  char **extension;
#if defined(NUS_DEBUG)
  // appends debug extension to list
  char **extensions_with_debug = malloc(sizeof(*extension_param) *
					(unsigned int)(extension_count + 1));
  for(i = 0; i < extension_count; ++i){
    extensions_with_debug[i] = malloc(sizeof(**extension_param) *
				      (unsigned int)(strlen(extension_param[i]) + 1));
    strcpy(extensions_with_debug[i], extension_param[i]);
  }
  char * const debug_extension = {
    "VK_EXT_debug_report"
  };
  extensions_with_debug[i] = malloc(strlen(debug_extension) + 1);
  strcpy(extensions_with_debug[i], debug_extension);

  extension = extensions_with_debug;
  NUS_VULKAN_INSTANCE_ADD_INFO(NUS_vulkan_instance_, extension,
			       (extension_count + 1), i);

  for(i = 0; i < extension_count + 1; ++i){
    free(extensions_with_debug[i]);
  }
  free(extensions_with_debug);
#elif !defined(NUS_DEBUG)
  extension = extension_param;
  NUS_VULKAN_INSTANCE_ADD_INFO(NUS_vulkan_instance_, extension, extension_count, i);
#endif
  return NUS_SUCCESS;
}
NUS_result nus_vulkan_instance_set_layers
(unsigned int layer_count, char **layer_param,
 NUS_vulkan_instance *NUS_vulkan_instance_)
{
  unsigned char i;
  char **layer;
#if defined(NUS_DEBUG)
  // appends debug layer to list
  char **layers_with_debug = malloc(sizeof(*layer_param) *
				    (unsigned int)(layer_count + 1));
  for(i = 0; i < layer_count; ++i){
    layers_with_debug[i] = malloc(sizeof(**layer_param) *
				      (unsigned int)(strlen(layer_param[i]) + 1));
    strcpy(layers_with_debug[i], layer_param[i]);
  }
  char * const debug_layer = {
    "VK_LAYER_LUNARG_standard_validation"
  };
  layers_with_debug[i] = malloc(strlen(debug_layer) + 1);
  strcpy(layers_with_debug[i], debug_layer);

  layer = layers_with_debug;
  NUS_VULKAN_INSTANCE_ADD_INFO(NUS_vulkan_instance_, layer,
			       (layer_count + 1), i);

  for(i = 0; i < layer_count + 1; ++i){
    free(layers_with_debug[i]);
  }
  free(layers_with_debug);
#elif !defined(NUS_DEBUG)
  layer = layer_param;
  NUS_VULKAN_INSTANCE_ADD_INFO(NUS_vulkan_instance_, layer, layer_count, i);
#endif
  return NUS_SUCCESS;
}
#if defined(NUS_DEBUG)
static VKAPI_ATTR VkBool32 VKAPI_CALL nus_validation_debug_callback
( VkDebugReportFlagsEXT flags,
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
