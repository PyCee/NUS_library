#ifndef NUS_VULKAN_INSTANCE_H
#define NUS_VULKAN_INSTANCE_H

#define NUS_INSTANCE_EXTENSION_SURFACE 0x01
#define NUS_INSTANCE_EXTENSION_OTHER 0x02

#include "../NUS_vulkan.h"
#include "../NUS_result.h"
#include "../NUS_string_group.h"

typedef struct NUS_vulkan_instance{
  NUS_vk_instance_functions functions;
  VkInstance instance;
} NUS_vulkan_instance;

NUS_result nus_vulkan_instance_build
(NUS_vulkan_instance *, NUS_string_group, NUS_string_group);
void nus_vulkan_instance_free(NUS_vulkan_instance *);

#endif /* NUS_VULKAN_INSTANCE_H */
