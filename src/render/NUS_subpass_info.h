#ifndef NUS_SUBPASS_INFO_H
#define NUS_SUBPASS_INFO_H

#include "../NUS_vulkan.h"
#include "../NUS_result.h"

#define NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT 20

typedef struct NUS_subpass_info{
  // Assumes vulkan subpass resolve attachment is NULL
  VkAttachmentReference p_input_attachment_refs[NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT],
    p_color_attachment_refs[NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT],
    p_depth_stencil_attachment_ref[1],
    p_preserve_attachment_refs[NUS_SUBPASS_INFO_MAX_ATTACHMENT_COUNT];
  unsigned int input_ref_count,
    color_ref_count,
    preserve_ref_count;
} NUS_subpass_info;

NUS_result nus_subpass_info_build
(unsigned int, unsigned int, unsigned int, NUS_subpass_info *);
void nus_subpass_info_free(NUS_subpass_info *);
/*
NUS_result nus_subpass_info_set_input_ref
(unsigned int, VkImageLayout, unsigned int, NUS_subpass_info *);
NUS_result nus_subpass_info_set_color_ref
(unsigned int, VkImageLayout, unsigned int, NUS_subpass_info *);
NUS_result nus_subpass_info_set_depth_stencil_ref
(unsigned int, VkImageLayout, NUS_subpass_info *);
NUS_result nus_subpass_info_set_preserve_ref
(unsigned int, VkImageLayout, unsigned int, NUS_subpass_info *);
*/
#endif /* NUS_SUBPASS_INFO_H */
