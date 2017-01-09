#ifndef _NUS_GPU_GROUP_H_
#define _NUS_GPU_GROUP_H_

#include "../NUS_result.h"
#include <vulkan/vulkan.h>


/* Bits for queue family supported operations flag */
#define NUS_QUEUE_SUPPORT_GRAPHICS 0x01 /* supports graphics operations */
#define NUS_QUEUE_SUPPORT_COMPUTE 0x02 /* supports compute operations */
#define NUS_QUEUE_SUPPORT_TRANSFER 0x04 /* supports transfer operations */
#define NUS_QUEUE_SUPPORT_SPARSE 0x08 /* supports sparse operations */
#define NUS_QUEUE_SUPPORT_PRESENT 0x10 /* supports present/surface  operations */

typedef struct NUS_gpu_queue_family{
  float *queue_family_priorities;
  VkQueue *queues;
  VkDeviceQueueCreateInfo queue_create_info;
  unsigned int queue_count,
    flags;
} NUS_gpu_queue_family;

typedef struct NUS_gpu{
  VkExtensionProperties *extension_properties;
  VkQueueFamilyProperties *queue_family_properties;
  NUS_gpu_queue_family *queue_families;
  VkDevice device;
  unsigned int queue_family_count,
    extension_property_count;
} NUS_gpu;

typedef struct NUS_gpu_group{
  VkPhysicalDevice *physical_devices;
  NUS_gpu *gpus;/* dynamically allocated group of gpus available to the application*/
  unsigned int gpu_count;
} NUS_gpu_group;

NUS_result nus_build_gpu_group(VkInstance, NUS_gpu_group *);
void nus_free_gpu_group(NUS_gpu_group *);
void nus_print_gpu_group(NUS_gpu_group);
NUS_result nus_check_gpu_presentation_support(VkSurfaceKHR, NUS_gpu_group *);

#endif /* _NUS_GPU_GROUP_H_ */
