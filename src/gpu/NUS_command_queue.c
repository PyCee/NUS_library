#include "NUS_command_queue.h"

NUS_result nus_command_queue_build
(VkDevice device, unsigned int family_index, unsigned int queue_index,
 NUS_command_queue *NUS_command_queue_)
{
  vkGetDeviceQueue(device, family_index, queue_index, &NUS_command_queue_->queue);
  NUS_command_queue_->workload = 0;
  return NUS_SUCCESS;
}
void nus_command_queue_free(NUS_command_queue *NUS_command_queue_)
{

}
