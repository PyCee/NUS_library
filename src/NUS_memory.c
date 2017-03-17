#include "NUS_memory.h"
#include "gpu/NUS_suitable_queue.h"

unsigned int nus_vk_memory_type_index
(struct NUS_suitable_queue info, VkMemoryRequirements mem_req, unsigned int properties)
{
  unsigned int i;
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(info.p_gpu->physical_device,
				      &mem_properties);
  for(i = 0; i < mem_properties.memoryTypeCount; ++i){
    if((mem_req.memoryTypeBits & (uint32_t)(1 << i)) &&
       (mem_properties.memoryTypes[i].propertyFlags & properties)){
      break;
    }
  }
  return i;
}
