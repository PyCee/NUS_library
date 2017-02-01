#ifndef NUS_GPU_H
#define NUS_GPU_H

#include "../NUS_vulkan.h"
#include "NUS_queue_family.h"

typedef struct NUS_gpu{
  NUS_queue_family *queue_families;
  VkExtensionProperties *extension_properties;
  VkQueueFamilyProperties *family_properties;
  NUS_vk_device_functions functions;
  VkDevice logical_device;
  unsigned int queue_family_count,
    extension_property_count;
} NUS_gpu;

NUS_result nus_gpu_build(VkPhysicalDevice, NUS_gpu *);
void nus_gpu_free(NUS_gpu *);
NUS_result nus_gpu_build_logical_device(VkPhysicalDevice, NUS_gpu *);
void nus_gpu_print(NUS_gpu);
NUS_result nus_gpu_find_suitable_queue
(NUS_gpu, unsigned int, unsigned int *, VkQueue *);

#endif /* NUS_GPU_H */
