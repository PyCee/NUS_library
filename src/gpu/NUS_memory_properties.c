#include "NUS_memory_properties.h"
#include "NUS_gpu.h"
#include <limits.h>

unsigned int nus_memory_properties_type_index
(NUS_gpu gpu, VkMemoryRequirements mem_req, unsigned int type)
{
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(gpu.physical_device, &mem_properties);
  unsigned int i;
  for(i = 0; i < mem_properties.memoryTypeCount; ++i){
    if((mem_req.memoryTypeBits & (uint32_t)(1 << i)) &&
       (mem_properties.memoryTypes[i].propertyFlags & type)){
      return i;
    }
  }
  return UINT_MAX;
}
