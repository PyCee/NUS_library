#include "NUS_image_view.h"
#include "../NUS_log.h"
#include "NUS_texture.h"

NUS_result nus_image_view_build
(NUS_image_view *p_image_view, NUS_texture texture, VkImageAspectFlags aspect_flags)
{
  VkImageViewCreateInfo view_create_info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .image = texture.image,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = texture.format,
    .components = {
      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      .a = VK_COMPONENT_SWIZZLE_IDENTITY
    },
    .subresourceRange = {
      .aspectMask = aspect_flags,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1
    }
  };
  if(vkCreateImageView(nus_get_bound_device(), &view_create_info, NULL,
		       &p_image_view->vk_image_view) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to create image view\n");
    return NUS_FAILURE;
  }
  p_image_view->binding = nus_get_binding();
  return NUS_SUCCESS;
}
void nus_image_view_free(NUS_image_view *p_image_view)
{
  nus_bind_binding(&p_image_view->binding);
  vkDestroyImageView(nus_get_bound_device(), p_image_view->vk_image_view, NULL);
  p_image_view->vk_image_view = VK_NULL_HANDLE;
  nus_unbind_binding(&p_image_view->binding);
}
