#include "NUS_texture.h"
#include "../gpu/NUS_gpu.h"
#include <stdio.h>

NUS_result nus_texture_build
(NUS_gpu gpu, unsigned int width, unsigned int height,
 VkFormat format, unsigned int usage, NUS_texture *p_texture)
{
  VkImageCreateInfo image_create_info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .imageType = VK_IMAGE_TYPE_2D,
    .format = format,
    .extent = {
      .width = width,
      .height = height,
      .depth = 1
    },
    .mipLevels = 1,
    .arrayLayers = 1,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .tiling = VK_IMAGE_TILING_OPTIMAL,
    .usage = usage,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED
  };
  if(vkCreateImage(gpu.logical_device, &image_create_info, NULL,
		   &p_texture->image) != VK_SUCCESS){
    printf("ERROR::failed to create texture image\n");
    return NUS_FAILURE;
  }
  VkMemoryRequirements image_memory_req;
  vkGetImageMemoryRequirements(gpu.logical_device, p_texture->image, &image_memory_req);
  
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(gpu.physical_device, &mem_properties);
  
  unsigned int memory_type_index;
  for(memory_type_index = 0; memory_type_index < mem_properties.memoryTypeCount;
      ++memory_type_index){
    if((image_memory_req.memoryTypeBits & (uint32_t)(1 << memory_type_index)) &&
       (mem_properties.memoryTypes[memory_type_index].propertyFlags &
        (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))){
      break;
    }
  }
  
  VkMemoryAllocateInfo memory_alloc_info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = image_memory_req.size,
    .memoryTypeIndex = memory_type_index
  };
  
  if(vkAllocateMemory(gpu.logical_device, &memory_alloc_info, NULL,
		      &p_texture->memory) != VK_SUCCESS){
    printf("ERROR::failed to allocate memory for texture\n");
    return NUS_FAILURE;
  }
  vkBindImageMemory(gpu.logical_device, p_texture->image, p_texture->memory, 0);
  return NUS_SUCCESS;
}
void nus_texture_free(NUS_gpu gpu, NUS_texture *p_texture)
{
  if(p_texture->memory != VK_NULL_HANDLE){
    vkFreeMemory(gpu.logical_device, p_texture->memory, NULL);
    p_texture->memory = VK_NULL_HANDLE;
  }
  if(p_texture->image != VK_NULL_HANDLE){
    vkDestroyImage(gpu.logical_device, p_texture->image, NULL);
    p_texture->image = VK_NULL_HANDLE;
  }
}
NUS_result nus_texture_load_image_file(NUS_texture *p_texture, char *file_path)
{
  // load image at file_path into p_texture
  return NUS_SUCCESS;
}
