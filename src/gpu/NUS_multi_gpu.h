#ifndef NUS_MULTI_GPU_H
#define NUS_MULTI_GPU_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "NUS_gpu.h"

struct NUS_vulkan_instance;
struct NUS_suitable_queue_info;

typedef struct NUS_multi_gpu{
  VkPhysicalDevice *physical_devices;
  NUS_gpu *gpus;
  unsigned int gpu_count;
} NUS_multi_gpu;

NUS_result nus_multi_gpu_build(struct NUS_vulkan_instance, NUS_multi_gpu *);
void nus_multi_gpu_free(NUS_multi_gpu *);
void nus_multi_gpu_print(NUS_multi_gpu);
NUS_result nus_multi_gpu_check_surface_support(VkSurfaceKHR, NUS_multi_gpu *);
NUS_result nus_multi_gpu_find_suitable_queue
(NUS_multi_gpu, unsigned int, struct NUS_suitable_queue_info *);
NUS_result nus_multi_gpu_submit_commands(NUS_multi_gpu);

#endif /* NUS_MULTI_GPU_H */
