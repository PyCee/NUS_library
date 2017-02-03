#ifndef _NUS_VULKAN_INSTANCE_H_
#define _NUS_VULKAN_INSTANCE_H_

#define NUS_INSTANCE_EXTENSION_SURFACE 0x01
#define NUS_INSTANCE_EXTENSION_OTHER 0x02

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

typedef struct NUS_vulkan_instance{
  char **extensions,
    **layers;
  NUS_vk_instance_functions functions;
  VkInstance instance;
  unsigned char extension_count,
    layer_count;
} NUS_vulkan_instance;

NUS_result nus_vulkan_instance_build(NUS_vulkan_instance *);
void nus_vulkan_instance_free(NUS_vulkan_instance *);

NUS_result nus_vulkan_instance_set_extensions
(unsigned char, char **, NUS_vulkan_instance *);
NUS_result nus_vulkan_instance_set_layers
(unsigned char, char **, NUS_vulkan_instance *);

#endif /* _NUS_VULKAN_INSTANCE_H_ */
