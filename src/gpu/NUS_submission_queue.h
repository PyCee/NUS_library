#ifndef NUS_SUBMISSION_QUEUE_H
#define NUS_SUBMISSION_QUEUE_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

#define NUS_SUBMISSION_QUEUE_BUFFER_COUNT 10
#define NUS_SUBMISSION_QUEUE_SIGNAL_COUNT 10
#define NUS_SUBMISSION_QUEUE_WAIT_COUNT 10
#define NUS_SUBMISSION_QUEUE_WAIT_TIMEOUT 16000000 // 16,000,000 nanoseconds, 16ms

typedef struct NUS_submission_queue{
  VkFence queue_finished;
  VkQueue queue;
  VkSubmitInfo info;
  VkSemaphore wait_sem[NUS_SUBMISSION_QUEUE_WAIT_COUNT],
    signal_sem[NUS_SUBMISSION_QUEUE_SIGNAL_COUNT];
  VkPipelineStageFlags wait_dst_mask[NUS_SUBMISSION_QUEUE_WAIT_COUNT];
  VkCommandBuffer buffer[NUS_SUBMISSION_QUEUE_BUFFER_COUNT];
} NUS_submission_queue;

NUS_result nus_submission_queue_build
(VkDevice, unsigned int, unsigned int, NUS_submission_queue *);
void nus_submission_queue_free(NUS_submission_queue *, VkDevice);
NUS_result nus_submission_queue_add_wait_semaphore
(NUS_submission_queue *, VkSemaphore, VkPipelineStageFlags);
NUS_result nus_submission_queue_add_signal_semaphore
(NUS_submission_queue *, VkSemaphore);
NUS_result nus_submission_queue_add_buffer(NUS_submission_queue *, VkCommandBuffer);
NUS_result nus_submission_queue_submit(NUS_submission_queue *, VkDevice);
NUS_result nus_submission_queue_wait(NUS_submission_queue *, VkDevice);

#endif /* NUS_SUBMISSION_QUEUE_H */
