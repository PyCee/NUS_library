#include "NUS_vulkan_instance.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static NUS_result nus_check_vulkan_instance_support(NUS_vulkan_instance);
static NUS_result nus_check_extension_support(char **, unsigned char);
static NUS_result nus_check_layer_support(char **, unsigned char);

void nus_init_vulkan_instance(NUS_vulkan_instance *NUS_vulkan_instance_)
{
  NUS_vulkan_instance_->extensions = NULL;
  NUS_vulkan_instance_->layers = NULL;
  NUS_vulkan_instance_->instance = VK_NULL_HANDLE;
  NUS_vulkan_instance_->extension_count = 0;
  NUS_vulkan_instance_->layer_count = 0;
}
NUS_result nus_build_vulkan_instance
(NUS_vulkan_instance *NUS_vulkan_instance_)
{
  VkApplicationInfo application_create_info;
  VkInstanceCreateInfo instance_create_info;

  const char * const *extensions =
    (const char * const *)NUS_vulkan_instance_->extensions;
  const char * const *layers =
    (const char * const *)NUS_vulkan_instance_->layers;
  
  if(nus_check_vulkan_instance_support(*NUS_vulkan_instance_) != NUS_SUCCESS){
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
  return NUS_SUCCESS;
}
void nus_free_vulkan_instance(NUS_vulkan_instance *NUS_vulkan_instance_)
{
  unsigned char i;
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

static NUS_result nus_check_vulkan_instance_support
(NUS_vulkan_instance NUS_vulkan_instance_)
{
  if(nus_check_extension_support(NUS_vulkan_instance_.extensions,
				 NUS_vulkan_instance_.extension_count)
     != NUS_SUCCESS ||
     nus_check_layer_support(NUS_vulkan_instance_.layers,
			     NUS_vulkan_instance_.layer_count) != NUS_SUCCESS){
    printf("ERROR::vulkan instance info is not supported\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
static NUS_result nus_check_extension_support
(char **extensions, unsigned char extension_count)
{
  if(extension_count > 0){
    unsigned int supported_extension_count;
    unsigned char i,
      j;
    supported_extension_count = 0;
    if(vkEnumerateInstanceExtensionProperties(NULL, &supported_extension_count,
					      NULL) != VK_SUCCESS ||
       supported_extension_count == 0){
      printf("ERROR::instance extension enumeration\n");
      return NUS_FAILURE;
    }
    VkExtensionProperties supported_extensions[supported_extension_count];
    if(vkEnumerateInstanceExtensionProperties(NULL, &supported_extension_count,
					      supported_extensions) != VK_SUCCESS){
      printf("ERROR::instance extension enumeration\n");
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
static NUS_result nus_check_layer_support(char **layers, unsigned char layer_count)
{
  if(layer_count > 0){
    unsigned int supported_layer_count;
    unsigned char i,
      j;
    supported_layer_count = 0;
    if(vkEnumerateInstanceLayerProperties(&supported_layer_count,
					  NULL) != VK_SUCCESS ||
       supported_layer_count == 0){
      printf("ERROR::instance extension enumeration\n");
      return NUS_FAILURE;
    }
    VkLayerProperties supported_layers[supported_layer_count];
    if(vkEnumerateInstanceLayerProperties(&supported_layer_count,
					  supported_layers) != VK_SUCCESS){
      printf("ERROR::instance layer enumeration\n");
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

#define NUS_VULKAN_INSTANCE_ADD_INFO(p_instance, type, i)		\
  do{									\
    i = p_instance->type##_count++;					\
    if(NULL == type){							\
      printf("ERROR::no %s passed\n", type);				\
      return NUS_FAILURE;						\
    }									\
    p_instance->type##s = realloc(p_instance->type##s,			\
				  sizeof(*p_instance->type##s) *	\
				  p_instance->type##_count);		\
    p_instance->type##s[i] = malloc(strlen(type) + 1);			\
    strcpy(p_instance->type##s[i], type);				\
  }while(0)

NUS_result nus_vulkan_instance_add_extension
(char *extension, NUS_vulkan_instance *NUS_vulkan_instance_)
{
  unsigned char i;
  NUS_VULKAN_INSTANCE_ADD_INFO(NUS_vulkan_instance_, extension, i);
  return NUS_SUCCESS;
}
NUS_result nus_vulkan_instance_add_layer
(char *layer, NUS_vulkan_instance *NUS_vulkan_instance_)
{
  unsigned char i;
  NUS_VULKAN_INSTANCE_ADD_INFO(NUS_vulkan_instance_, layer, i);
  return NUS_SUCCESS;
}
