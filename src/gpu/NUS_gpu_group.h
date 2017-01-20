#ifndef _NUS_GPU_GROUP_H_
#define _NUS_GPU_GROUP_H_

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "NUS_gpu.h"

typedef struct NUS_gpu_group{
  VkPhysicalDevice *physical_devices;
  NUS_gpu *gpus;
  unsigned int gpu_count;
} NUS_gpu_group;

NUS_result nus_gpu_group_build(VkInstance, NUS_gpu_group *);
void nus_gpu_group_free(NUS_gpu_group *);
void nus_gpu_group_print(NUS_gpu_group);
NUS_result nus_gpu_group_check_surface_support(VkSurfaceKHR, NUS_gpu_group *);

#endif /* _NUS_GPU_GROUP_H_ */
