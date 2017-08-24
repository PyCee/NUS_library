
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#define NUS_SYSTEM_RENDER
#include <NUS/NUS_library.h>
#include <limits.h>

#include <unistd.h>

#define PROGRAM_NAME "unit_test-texture"

void close_win(void);

char run;


int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  nus_load_global_vulkan_library();
  
  NUS_window win;
  if(nus_window_build(PROGRAM_NAME, 600, 400, &win) != NUS_SUCCESS){
    printf("ERROR::failed to create window\n");
    return -1;
  }
  NUS_event_handler eve;
  if(nus_event_handler_build(&eve) != NUS_SUCCESS){
    printf("ERROR::failed to build event handler\n");
    return -1;
  }
  nus_event_handler_append(eve, NUS_EVENT_CLOSE_WINDOW, 0, close_win);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_ESC, close_win);
  
  nus_event_handler_set(&eve);
  
  NUS_vulkan_instance vulkan_instance;
  
  NUS_string_group instance_extensions;
  nus_string_group_build(&instance_extensions);
  nus_string_group_append(&instance_extensions, 
			  VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(NUS_OS_WINDOWS)
  nus_string_group_append(&instance_extensions, 
			  VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(NUS_OS_UNIX)
  nus_string_group_append(&instance_extensions,
			  VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
  NUS_string_group instance_layers;
  nus_string_group_build(&instance_layers);
  
  if(nus_vulkan_instance_build(&vulkan_instance, instance_extensions,
			       instance_layers) != NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }
  nus_string_group_free(&instance_extensions);
  nus_string_group_free(&instance_layers);
  
  
  NUS_multi_gpu multi_gpu;
  if(nus_multi_gpu_build(vulkan_instance, &multi_gpu) != NUS_SUCCESS){
    printf("ERROR::build multi gpu returned NUS_FAILURE\n");
    return -1;
  }

  NUS_presentation_surface present;
  if(nus_presentation_surface_build(win, vulkan_instance, &multi_gpu, &present)
     != NUS_SUCCESS){
    printf("ERROR::failed to build presentaion surface\n");
    return -1;
  }
  
  // temp init code
  
  NUS_model model;
  // the vertex normal represents color for this unit test
  if(nus_model_build(nus_absolute_path_build("triangle.nusm"), &model) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to build model\n");
    return -1;
  }

  // code to create renderpass
  VkAttachmentDescription attachment_descriptions[] = {
    {
      .flags = 0,
      .format = present.swapchain.format.format,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    }
  };
  VkAttachmentReference color_attachment_references[] = {
    {
      .attachment = 0,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    }
  };
  VkSubpassDescription subpass_descriptions[] = {
    {
      .flags = 0,
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount = 0,
      .pInputAttachments = NULL,
      .colorAttachmentCount = 1,
      .pColorAttachments = color_attachment_references,
      .pResolveAttachments = NULL,
      .pDepthStencilAttachment = NULL,
      .preserveAttachmentCount = 0,
      .pPreserveAttachments = NULL
    }
  };
  
  VkSubpassDependency dependencies[] = {
    {
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
    },
    {
      .srcSubpass = 0,
      .dstSubpass = VK_SUBPASS_EXTERNAL,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
    }
  };
  
  VkRenderPassCreateInfo render_pass_create_info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .attachmentCount = 1,
    .pAttachments = attachment_descriptions,
    .subpassCount = 1,
    .pSubpasses = subpass_descriptions,
    .dependencyCount = 2,
    .pDependencies = dependencies
  };

  NUS_render_pass render_pass;
  if(nus_render_pass_build(render_pass_create_info, &render_pass) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to build render pass\n");
    return -1;
  }

  // Framebuffer creation
  NUS_image_view framebuffer_img_views[1];
  if(nus_image_view_build(framebuffer_img_views + 0, present.render_target,
			  VK_IMAGE_ASPECT_COLOR_BIT) != NUS_SUCCESS){
    NUS_LOG_ERROR("failed to build framebuffer image views\n");
    return -1;
  }
  
  NUS_framebuffer framebuffer;
  if(nus_framebuffer_build(&framebuffer, render_pass, framebuffer_img_views, 1,
			   600, 400) != NUS_SUCCESS){
    printf("ERROR::failed to build framebuffer\n");
    return -1;
  }
  

  // Create Graphics Pipeline
  NUS_shader vertex_shader,
    fragment_shader;
  if(nus_shader_build(nus_absolute_path_build("triangle_shader/shader.vert.spv"),
		      VK_SHADER_STAGE_VERTEX_BIT, &vertex_shader) != NUS_SUCCESS){
    printf("ERROR::failed to build vertex shader\n");
    return -1;
  }
  if(nus_shader_build(nus_absolute_path_build("triangle_shader/shader.frag.spv"),
		      VK_SHADER_STAGE_FRAGMENT_BIT, &fragment_shader) != NUS_SUCCESS){
    printf("ERROR::failed to build fragment shader\n");
    return -1;
  }
  
  VkPipelineShaderStageCreateInfo shader_stage_create_info[] = {
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .stage = vertex_shader.stage,
      .module = vertex_shader.module,
      .pName = "main",
      .pSpecializationInfo = NULL
    },
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .stage = fragment_shader.stage,
      .module = fragment_shader.module,
      .pName = "main",
      .pSpecializationInfo = NULL
    }
  };

  VkVertexInputBindingDescription vertex_binding_description = {
    .binding = 0,//
    .stride = (unsigned int)sizeof(NUS_vertex),//
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX//
  };
  VkVertexInputAttributeDescription vertex_attribute_description[] = {
    {
      .location = 0,//
      .binding = vertex_binding_description.binding,//
      .format = VK_FORMAT_R32G32B32_SFLOAT,//
      .offset = sizeof(float) * 0//
    },
    {
      .location = 1,//
      .binding = vertex_binding_description.binding,//
      .format = VK_FORMAT_R32G32B32_SFLOAT,//
      .offset = sizeof(float) * 3//
    },
    {
      .location = 2,//
      .binding = vertex_binding_description.binding,//
      .format = VK_FORMAT_R32G32_SFLOAT,//
      .offset = sizeof(float) * 6//
    }
  };
  
  VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .vertexBindingDescriptionCount = 1,
    .pVertexBindingDescriptions = &vertex_binding_description,
    .vertexAttributeDescriptionCount = 3,
    .pVertexAttributeDescriptions = vertex_attribute_description
  };
  
  VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,//
    .primitiveRestartEnable = VK_FALSE
  };
  
  VkPipelineViewportStateCreateInfo viewport_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    // The below variables allow for a dynamic viewport state
    .viewportCount = 1,
    .pViewports = NULL,
    .scissorCount = 1,
    .pScissors = NULL
  };
  
  VkDynamicState dynamics_states[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };
  
  VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .dynamicStateCount = 2,//
    .pDynamicStates = dynamics_states//
  };
  
  VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .depthClampEnable = VK_FALSE,//
    .rasterizerDiscardEnable = VK_FALSE,//
    .polygonMode = VK_POLYGON_MODE_FILL,//
    .cullMode = VK_CULL_MODE_BACK_BIT,//
    .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,//
    .depthBiasEnable = VK_FALSE,//
    .depthBiasConstantFactor = 0.0f,//
    .depthBiasClamp = 0.0f,//
    .depthBiasSlopeFactor = 0.0f,//
    .lineWidth = 1.0f//
  };
  
  VkPipelineMultisampleStateCreateInfo multisample_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,//
    .sampleShadingEnable = VK_FALSE,//
    .minSampleShading = 1.0f,//
    .pSampleMask = NULL,//
    .alphaToCoverageEnable = VK_FALSE,//
    .alphaToOneEnable = VK_FALSE//
  };
  
  VkPipelineColorBlendAttachmentState color_blend_attachment_state = {
    .blendEnable = VK_FALSE,//
    .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,//
    .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,//
    .colorBlendOp = VK_BLEND_OP_ADD,//
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,//
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,//
    .alphaBlendOp = VK_BLEND_OP_ADD,//
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT//
  };
  
  VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .logicOpEnable = VK_FALSE,//
    .logicOp = VK_LOGIC_OP_COPY,//
    .attachmentCount = 1,//
    .pAttachments = &color_blend_attachment_state,//
    .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}//
  };
  
  VkDescriptorSetLayoutBinding desc_set_layout_bindings[] = {
    {
      .binding = 0,
      .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      .descriptorCount = 1,
      .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
      .pImmutableSamplers = NULL
    }
  };
    
  VkDescriptorSetLayoutCreateInfo desc_set_layout_create_info = {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .bindingCount = 1,
      .pBindings = desc_set_layout_bindings
  };
  VkDescriptorSetLayout desc_set_layout;
  vkCreateDescriptorSetLayout(nus_get_bound_device(), &desc_set_layout_create_info,
			      NULL, &desc_set_layout);
    
  VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .setLayoutCount = 1,
    .pSetLayouts = &desc_set_layout,
    .pushConstantRangeCount = 0,//
    .pPushConstantRanges = NULL//
  };

  NUS_pipeline_layout pipeline_layout;
  if(nus_pipeline_layout_build(pipeline_layout_create_info, &pipeline_layout) !=
     NUS_SUCCESS){
    NUS_LOG_ERROR("failed to create pipeline layout\n");
    return -1;
  }
  
  VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .stageCount = 2,
    .pStages = shader_stage_create_info,
    .pVertexInputState = &vertex_input_state_create_info,
    .pInputAssemblyState = &input_assembly_state_create_info,
    .pTessellationState = NULL,
    .pViewportState = &viewport_state_create_info,
    .pRasterizationState = &rasterization_state_create_info,
    .pMultisampleState = &multisample_state_create_info,
    .pDepthStencilState = NULL,
    .pColorBlendState = &color_blend_state_create_info,
    .pDynamicState = &dynamic_state_create_info,
    .layout = nus_pipeline_layout_get_raw(pipeline_layout),
    .renderPass = nus_render_pass_get_raw(render_pass),
    .basePipelineHandle = VK_NULL_HANDLE,
    .basePipelineIndex = -1
  };

  NUS_graphics_pipeline graphics_pipeline;
  if(nus_graphics_pipeline_build(graphics_pipeline_create_info, &graphics_pipeline) !=
     NUS_SUCCESS){
    NUS_LOG_ERROR("failed to create graphics pipelines\n");
    return -1;
  }

  // end of render pass init
  
  
  VkDescriptorPoolSize pool_sizes[] = {
    {
      .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      .descriptorCount = 1
    }
  };
  
  VkDescriptorPoolCreateInfo desc_pool_create_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .maxSets = 1,
    .poolSizeCount = 1,
    .pPoolSizes = pool_sizes
  };
  
  VkDescriptorPool descriptor_pool;
  if(vkCreateDescriptorPool(nus_get_bound_device(), &desc_pool_create_info,
			    NULL, &descriptor_pool) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to create descriptor pool\n");
    return -1;
  }
  
  VkDescriptorSetAllocateInfo desc_set_alloc_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext = NULL,
    .descriptorPool = descriptor_pool,
    .descriptorSetCount = 1,
    .pSetLayouts = &desc_set_layout
  };

  VkDescriptorSet descriptor_set;
  if(vkAllocateDescriptorSets(nus_get_bound_device(), &desc_set_alloc_info,
			      &descriptor_set) != VK_SUCCESS){
    NUS_LOG_ERROR("failed to allocate descriptor set\n");
    return -1;
  }


  NUS_image_view texture_img_view;
  if(nus_image_view_build(&texture_img_view, model.texture,
			  VK_IMAGE_ASPECT_COLOR_BIT) != NUS_SUCCESS){
    return -1;
  }
  NUS_sampler sampler;
  if(nus_sampler_build(&sampler, VK_FILTER_LINEAR) != NUS_SUCCESS){
    return -1;
  }
  VkDescriptorImageInfo desc_image_info = {
    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    .imageView = texture_img_view.vk_image_view,
    .sampler = sampler.vk_sampler
  };
  
  VkWriteDescriptorSet write_desc_set = {
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .pNext = NULL,
    .dstSet = descriptor_set,
    .dstBinding = 0,
    .dstArrayElement = 0,
    .descriptorCount = 1,
    .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    .pImageInfo = &desc_image_info,
    .pBufferInfo = NULL,
    .pTexelBufferView = NULL
  };
  
  vkUpdateDescriptorSets(nus_get_bound_device(), 1, &write_desc_set, 0, NULL);
  
  VkCommandBuffer command_buffer;
  VkCommandBufferBeginInfo command_buffer_begin_info = {
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    NULL,
    VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
    NULL
  };
  VkImageSubresourceRange image_subresource_range = {
    VK_IMAGE_ASPECT_COLOR_BIT,
    0,
    1,
    0,
    1
  };
  VkClearValue clear_value = { .color = {{0.4f, 0.1f, 0.3f, 1.0f}} };

  VkRenderPassBeginInfo render_pass_begin_info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext = NULL,
    .renderPass = nus_render_pass_get_raw(render_pass),
    .framebuffer = framebuffer.vk_framebuffer,
    .renderArea = {
      .offset = {
	.x = 0,
	.y = 0
      },
      .extent = present.swapchain.extent
    },
    .clearValueCount = 1,
    .pClearValues = &clear_value
  };
  
  VkImageMemoryBarrier barrier_from_undefined_to_present = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = NULL,
    .srcAccessMask = 0,
    .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    .srcQueueFamilyIndex = nus_get_bound_queue_family_index(),
    .dstQueueFamilyIndex = nus_get_bound_queue_family_index(),
    .image = present.render_target.image,
    .subresourceRange = image_subresource_range
  };
  VkImageMemoryBarrier barrier_from_present_to_draw = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = NULL,
    .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
    .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    .srcQueueFamilyIndex = nus_get_bound_queue_family_index(),
    .dstQueueFamilyIndex = nus_get_bound_queue_family_index(),
    .image = present.render_target.image,
    .subresourceRange = image_subresource_range
  };
  VkImageMemoryBarrier barrier_from_draw_to_present = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = NULL,
    .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    .srcQueueFamilyIndex = nus_get_bound_queue_family_index(),
    .dstQueueFamilyIndex = nus_get_bound_queue_family_index(),
    .image = present.render_target.image,
    .subresourceRange = image_subresource_range
  };
  
  VkViewport viewport = {
    .x = 0,
    .y = 0,
    .width = (float)present.swapchain.extent.width,
    .height = (float)present.swapchain.extent.height,
    .minDepth = 0.0f,
    .maxDepth = 1.0f
  };
  VkRect2D scissor = {
    .offset = {
      .x = 0,
      .y = 0
    },
    .extent = present.swapchain.extent
  };
  nus_graphics_pipeline_set_viewport(viewport, &graphics_pipeline);
  nus_graphics_pipeline_set_scissor(scissor, &graphics_pipeline);
  
  nus_allocate_command_buffer(&command_buffer, 1);
  vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
  vkCmdPipelineBarrier(command_buffer,
		       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		       0, 0, NULL, 0, NULL, 1, &barrier_from_undefined_to_present);
  vkCmdPipelineBarrier(command_buffer,
		       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		       0, 0, NULL, 0, NULL, 1, &barrier_from_present_to_draw);
  
  vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info,
		       VK_SUBPASS_CONTENTS_INLINE);
  
  nus_cmd_graphics_pipeline_bind(command_buffer, graphics_pipeline);
  
  vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
			  nus_pipeline_layout_get_raw(pipeline_layout),
			  0, 1, &descriptor_set, 0, NULL);
  
  VkDeviceSize vertex_memory_offset = 0,
    index_memory_offset = 0;
  vkCmdBindVertexBuffers(command_buffer, 0, 1, &model.vertex_memory.buffer,
			 &vertex_memory_offset);
  vkCmdBindIndexBuffer(command_buffer, model.index_memory.buffer, index_memory_offset,
			VK_INDEX_TYPE_UINT32);

  vkCmdDrawIndexed(command_buffer, model.index_memory.size/sizeof(NUS_indice),
		   1, 0, 0, 0);
    
  vkCmdEndRenderPass(command_buffer);
  
  vkCmdPipelineBarrier(command_buffer,
		       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		       VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
		       0, 0, NULL, 0, NULL, 1, &barrier_from_draw_to_present);
  
  if(vkEndCommandBuffer(command_buffer) != VK_SUCCESS){
    printf("ERROR::Could not record command buffers!\n");
    return NUS_FAILURE;
  }
  
  run = 1;
  while(run){

    nus_system_events_handle(win);

    nus_add_wait_semaphore(present.image_available, VK_PIPELINE_STAGE_TRANSFER_BIT);
    nus_add_signal_semaphore(present.image_presentable);
    nus_add_command_buffer(command_buffer);
    nus_submit_queue();
    
    if(nus_multi_gpu_submit_commands(multi_gpu) != NUS_SUCCESS){
      printf("ERROR::failed to submit multi gpu command queues\n");
      return -1;
    }
    
    if(nus_presentation_surface_present(&present) != NUS_SUCCESS){
      printf("ERROR::failed to present window\n");
      return -1;
    }

    //run = 0;
  }
  
  printf("freeing unit test %s\n", PROGRAM_NAME);

  vkDeviceWaitIdle(nus_get_bound_device());
  
  vkDestroyDescriptorPool(nus_get_bound_device(), descriptor_pool, NULL);
  vkDestroyDescriptorSetLayout(nus_get_bound_device(), desc_set_layout, NULL);

  nus_sampler_free(&sampler);
  nus_image_view_free(&texture_img_view);
  
  nus_model_free(&model);
  
  nus_shader_free(&vertex_shader);
  nus_shader_free(&fragment_shader);
  
  nus_graphics_pipeline_free(&graphics_pipeline);
  nus_pipeline_layout_free(&pipeline_layout);
  
  nus_framebuffer_free(&framebuffer);
  nus_image_view_free(&framebuffer_img_views[0]);
  
  nus_render_pass_free(&render_pass);
  
  nus_presentation_surface_free(vulkan_instance, &present);
  nus_multi_gpu_free(&multi_gpu);
  nus_vulkan_instance_free(&vulkan_instance);
  nus_free_vulkan_library();
  
  nus_window_free(&win);
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
void close_win(void)
{
  run = 0;
}
