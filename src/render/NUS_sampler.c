#include "NUS_sampler.h"
#include "../NUS_log.h"

NUS_result nus_sampler_build
(NUS_sampler *p_sampler, VkFilter filter)
{
  VkSamplerCreateInfo sampler_create_info;
  *p_sampler = (NUS_sampler){
    .vk_sampler = VK_NULL_HANDLE,
    .binding = nus_get_binding()
  };
  sampler_create_info = (VkSamplerCreateInfo){
    .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .magFilter = filter,// VK_FILTER_LINEAR
    .minFilter = filter,// VK_FILTER_LINEAR
    .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,//TODO
    .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    .mipLodBias = 0.0f,//TODO
    .anisotropyEnable = VK_TRUE,
    .maxAnisotropy = NUS_DEFAULT_ANISOTROPY,
    .compareEnable = VK_FALSE,//TODO
    .compareOp = VK_COMPARE_OP_ALWAYS,//TODO
    .minLod = 0.0f,//TODO
    .maxLod = 0.0f,//TODO
    .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    .unnormalizedCoordinates = VK_FALSE
  };
  if(vkCreateSampler(nus_get_bound_device(), &sampler_create_info, NULL,
		     &p_sampler->vk_sampler) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to build sampler\n");
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
void nus_sampler_free(NUS_sampler *p_sampler)
{
  nus_bind_binding(&p_sampler->binding);
  vkDestroySampler(nus_get_bound_device(), p_sampler->vk_sampler, NULL);
  nus_unbind_binding(&p_sampler->binding);
}
