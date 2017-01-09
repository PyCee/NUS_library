#ifndef _NUS_VULKAN_INSTANCE_H_
#define _NUS_VULKAN_INSTANCE_H_

#define NUS_INSTANCE_EXTENSION_SURFACE 0x01
#define NUS_INSTANCE_EXTENSION_OTHER 0x02

#include "../NUS_result.h"
#include <vulkan/vulkan.h>

typedef struct NUS_vulkan_instance_info{
  char **extensions,
    **layers;
  unsigned char extension_count,
    layer_count;
} NUS_vulkan_instance_info;

typedef struct NUS_vulkan_instance{
  VkInstance instance;
  
} NUS_vulkan_instance;

NUS_result nus_build_vulkan_instance_info(NUS_vulkan_instance_info *);
NUS_result nus_vulkan_instance_info_add_extension(char *, NUS_vulkan_instance_info *);
NUS_result nus_vulkan_instance_info_add_layer(char *, NUS_vulkan_instance_info *);
void nus_free_vulkan_instance_info(NUS_vulkan_instance_info *);

NUS_result nus_build_vulkan_instance(NUS_vulkan_instance_info, NUS_vulkan_instance *);
void nus_free_vulkan_instance(NUS_vulkan_instance *);

#endif /* _NUS_VULKAN_INSTANCE_H_ */
