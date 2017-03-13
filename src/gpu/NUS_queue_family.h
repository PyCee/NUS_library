#ifndef NUS_QUEUE_FAMILY_H
#define NUS_QUEUE_FAMILY_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"
#include "NUS_command_queue.h"

/* Bits for queue family supported operations flag */
#define NUS_QUEUE_FAMILY_SUPPORT_GRAPHICS 0x01 /* supports graphics operations */
#define NUS_QUEUE_FAMILY_SUPPORT_COMPUTE 0x02 /* supports compute operations */
#define NUS_QUEUE_FAMILY_SUPPORT_TRANSFER 0x04 /* supports transfer operations */
#define NUS_QUEUE_FAMILY_SUPPORT_SPARSE 0x08 /* supports sparse operations */
#define NUS_QUEUE_FAMILY_SUPPORT_PRESENT 0x10 /* supports present/surface  operations */

typedef struct NUS_queue_family{
  NUS_command_queue *queues;
  VkCommandPool command_pool;
  unsigned int queue_count,
    family_index,
    flags;
} NUS_queue_family;

NUS_result nus_queue_family_build
(VkQueueFamilyProperties, unsigned int, NUS_queue_family *);
void nus_queue_family_free(NUS_queue_family *, VkDevice);
void nus_queue_family_print(NUS_queue_family);
NUS_result nus_queue_family_build_queues(VkDevice, NUS_queue_family *);
NUS_result nus_queue_family_test_surface_support
(VkPhysicalDevice, VkSurfaceKHR, NUS_queue_family *);
NUS_result nus_queue_family_find_suitable_queue
(NUS_queue_family, unsigned int *);
NUS_result nus_queue_family_add_command_buffer
(NUS_queue_family, VkDevice, VkCommandBuffer *);
NUS_result nus_queue_family_submit_commands(NUS_queue_family, VkDevice);

#endif /* NUS_QUEUE_FAMILY_H */
