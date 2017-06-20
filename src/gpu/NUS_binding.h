#ifndef NUS_BINDING_H
#define NUS_BINDING_H

#include "../NUS_result.h"
#include "../NUS_vulkan.h"

#include "NUS_gpu.h"
#include "NUS_queue_family.h"
#include "NUS_submission_queue.h"

struct NUS_multi_gpu;

typedef struct NUS_binding{
  NUS_gpu *p_gpu;
  NUS_queue_family *p_queue_family;
  NUS_submission_queue *p_submission_queue;
  unsigned int queue_requirements;
} NUS_binding;

NUS_binding nus_get_binding(void);
void nus_bind_binding(NUS_binding *);

/*
 * nus_unbind_binding preforms the same action as nus_bind_binding
 * It would be odd to bind a binding twice in one function
 * Better to bind and then unbind than to bind and then bind
 */
extern void (*nus_unbind_binding)(NUS_binding *);

void nus_set_queue_requirements(unsigned int);

NUS_result nus_select_gpu(struct NUS_multi_gpu);
NUS_result nus_select_queue_family(NUS_gpu);
NUS_result nus_select_submission_queue(NUS_queue_family);

NUS_gpu *nus_get_bound_gpu(void);
NUS_queue_family *nus_get_bound_queue_family(void);
NUS_submission_queue *nus_get_bound_submission_queue(void);
VkDevice nus_get_bound_device(void);

NUS_result nus_allocate_command_buffer(VkCommandBuffer *, char);
NUS_result nus_add_wait_semaphore(VkSemaphore, VkPipelineStageFlags);
NUS_result nus_add_signal_semaphore(VkSemaphore);
NUS_result nus_add_command_buffer(VkCommandBuffer);
NUS_result nus_submit_queue(void);
NUS_result nus_wait_on_queue(void);

#endif /* NUS_BINDING_H */
