#ifndef NUS_SAMPLER_H
#define NUS_SAMPLER_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"
#include "../gpu/NUS_binding.h"

#define NUS_DEFAULT_ANISOTROPY 16

typedef struct NUS_sampler{
  VkSampler vk_sampler;
  NUS_binding binding;
} NUS_sampler;

NUS_result nus_sampler_build(NUS_sampler *, VkFilter);
void nus_sampler_free(NUS_sampler *);

#endif /* NUS_SAMPLER_H */
