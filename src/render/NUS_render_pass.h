#ifndef NUS_RENDER_PASS_H
#define NUS_RENDER_PASS_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"
#include "NUS_subpass_info.h"

struct NUS_gpu;
/*
#define NUS_RENDER_PASS_INFO_MAX_ATTACHMENT_COUNT 15
#define NUS_RENDER_PASS_INFO_MAX_SUBPASS_COUNT 10
#define NUS_RENDER_PASS_INFO_MAX_DEPENDENCY_COUNT 30

typedef struct NUS_render_pass_info{
  VkAttachmentDescription p_attachment_descriptions
  [NUS_RENDER_PASS_INFO_MAX_ATTACHMENT_COUNT];
  VkSubpassDescription p_subpass_info[NUS_RENDER_PASS_INFO_MAX_SUBPASS_COUNT];
  VkSubpassDependency p_dependencies[NUS_RENDER_PASS_INFO_MAX_DEPENDENCY_COUNT];
  unsigned int attachment_description_count,
    subpass_count,
    dependency_count;
} NUS_render_pass_info;

NUS_result nus_render_pass_info_build
(unsigned int, unsigned int, unsigned int, NUS_render_pass_info *);
void nus_render_pass_info_free(struct NUS_gpu, NUS_render_pass_info *);
*/
/*
NUS_result nus_render_pass_info_set_attachment_description
(struct NUS_gpu, VkAttachmentDescription, unsigned int, NUS_render_pass_info *);
NUS_result nus_render_pass_info_set_subpass
(NUS_subpass_info,  nsigned int, NUS_render_pass_info *);
NUS_result nus_render_pass_info_set_dependency
(VkSubpassDependency, unsigned int, NUS_render_pass_info *);
*/
#endif /* NUS_RENDER_PASS_H */
