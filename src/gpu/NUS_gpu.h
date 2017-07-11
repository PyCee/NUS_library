#ifndef NUS_GPU_H
#define NUS_GPU_H

#include "../NUS_vulkan.h"
#include "../NUS_bool.h"
#include "NUS_queue_family.h"


typedef struct NUS_gpu{
  NUS_queue_family *queue_families;
  VkExtensionProperties *extension_properties;
  VkQueueFamilyProperties *family_properties;
  NUS_vk_device_functions functions;
  VkPhysicalDevice physical_device;
  VkDevice logical_device;
  VkPhysicalDeviceMemoryProperties memory_properties;
  unsigned int queue_family_count,
    extension_property_count;
} NUS_gpu;

NUS_result nus_gpu_build(VkPhysicalDevice, NUS_gpu *);
void nus_gpu_free(NUS_gpu *);
void nus_gpu_print(NUS_gpu);
NUS_result nus_gpu_submit_commands(NUS_gpu);
unsigned int nus_gpu_memory_type_index
(NUS_gpu, VkMemoryRequirements, VkMemoryPropertyFlags);
NUS_bool nus_gpu_qwery_format_support
(NUS_gpu, VkFormat, VkImageTiling, VkFormatFeatureFlags);

#endif /* NUS_GPU_H */
