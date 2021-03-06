\
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#define NUS_SYSTEM_RENDER
#include <NUS/NUS_library.h>
#include <limits.h>

#include <unistd.h>

#define PROGRAM_NAME "unit_test-load_model"

void close_win(void);
void move_forward(void);
void move_back(void);
void move_up(void);
void move_left(void);
void move_down(void);
void move_right(void);

void pitch_up(void);
void pitch_down(void);
void yaw_left(void);
void yaw_right(void);
char run;
float dx = 0.0, dy = 0.0, dz = 0.0, dpitch = 0.0, dyaw = 0.0;
float move_speed = 0.01, rotate_speed = 1.0 * 3.14159 / 180.0;

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
  
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS,
				    NUS_KEY_ESC, close_win);
  //nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_E, move_forward);
  //nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_Q, move_back);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_W, move_up);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_A, move_left);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_S, move_down);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_D, move_right);
  
  //nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE, NUS_KEY_E, move_back);
  //nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE, NUS_KEY_Q, move_forward);
  nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE, NUS_KEY_S, move_up);
  nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE, NUS_KEY_D, move_left);
  nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE, NUS_KEY_W, move_down);
  nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE, NUS_KEY_A, move_right);
  
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS,
				    NUS_KEY_ARROW_UP, pitch_up);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS,
				    NUS_KEY_ARROW_DOWN, pitch_down);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS,
				    NUS_KEY_ARROW_LEFT, yaw_left);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS,
				    NUS_KEY_ARROW_RIGHT, yaw_right);
  
  nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE,
				    NUS_KEY_ARROW_UP, pitch_down);
  nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE,
				    NUS_KEY_ARROW_DOWN, pitch_up);
  nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE,
				    NUS_KEY_ARROW_LEFT, yaw_right);
  nus_event_handler_append(eve, NUS_EVENT_KEY_RELEASE,
				    NUS_KEY_ARROW_RIGHT, yaw_left);
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
  
  NUS_queue_info info;
  
  if(nus_gpu_find_queue_info(present.queue_info.p_gpu,
				 NUS_QUEUE_FAMILY_SUPPORT_PRESENT |
				 NUS_QUEUE_FAMILY_SUPPORT_TRANSFER,
				 &info) !=
     NUS_SUCCESS){
    printf("ERROR::failed to find suitable gpu info\n");
    return NUS_FAILURE;
  }
  
  printf("start model\n");
  
  NUS_model model;
  
  nus_model_build(nus_absolute_path_build("cube.nusm"), &model);
  nus_model_buffer(info, &model);
  
  printf("end model stuffz\n");

  
  NUS_depth_buffer depth_buffer;
  if(nus_depth_buffer_build(*present.queue_info.p_gpu, 600, 400, &depth_buffer) !=
     NUS_SUCCESS){
    printf("UNIT_TEST_ERROR::failed to build depth_buffer\n");
    return -1;
  }
  nus_texture_initial_transition(depth_buffer, present.queue_info,
				 VK_IMAGE_LAYOUT_UNDEFINED,
				 VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  

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
    },
    {
      .flags = 0,
      .format = depth_buffer.format,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    }
  };
  VkAttachmentReference attachment_refs[] = {
    {
      .attachment = 0,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    },
    {
      .attachment = 1,
      .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    }
  };
  VkSubpassDescription subpass_descriptions[] = {
    {
      .flags = 0,
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount = 0,
      .pInputAttachments = NULL,
      .colorAttachmentCount = 1,
      .pColorAttachments = attachment_refs + 0,
      .pResolveAttachments = NULL,
      .pDepthStencilAttachment = attachment_refs + 1,
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
    .attachmentCount = 2,
    .pAttachments = attachment_descriptions,
    .subpassCount = 1,
    .pSubpasses = subpass_descriptions,
    .dependencyCount = 2,
    .pDependencies = dependencies
  };

  VkRenderPass render_pass;
  if(vkCreateRenderPass(present.queue_info.p_gpu->logical_device,
			&render_pass_create_info, NULL,
			&render_pass) != VK_SUCCESS){
    printf("ERROR::failed to create render pass\n");
    return -1;
  }
  
  // code to create framebuffer
  NUS_framebuffer_info framebuffer_info;
  nus_framebuffer_info_build(600, 400, 2, &framebuffer_info);
  if(nus_framebuffer_info_set_attachment(*present.queue_info.p_gpu,
					 present.render_target,
					 present.swapchain.format.format,
					 VK_IMAGE_ASPECT_COLOR_BIT, 0,
					 &framebuffer_info) != NUS_SUCCESS){
    printf("ERROR::failed to set framebuffer info attachment: render target\n");
    return -1;
  }
  
  if(nus_framebuffer_info_set_attachment(*present.queue_info.p_gpu,
					 depth_buffer,
					 present.swapchain.format.format,
					 VK_IMAGE_ASPECT_DEPTH_BIT, 1,
					 &framebuffer_info) != NUS_SUCCESS){
    printf("ERROR::failed to set framebuffer info attachment: render target\n");
    return -1;
  }

  NUS_framebuffer framebuffer;
  if(nus_framebuffer_build(*present.queue_info.p_gpu, render_pass, framebuffer_info,
			   &framebuffer) != NUS_SUCCESS){
    printf("ERROR::failed to build framebuffer\n");
    return -1;
  }

  // Create Graphics Pipeline
  NUS_shader vertex_shader,
    fragment_shader;
  if(nus_shader_build(multi_gpu.gpus[0],
		      nus_absolute_path_build("triangle_shader/shader.vert.spv"),
		      VK_SHADER_STAGE_VERTEX_BIT, &vertex_shader) != NUS_SUCCESS){
    printf("ERROR::failed to build vertex shader\n");
    return -1;
  }
  if(nus_shader_build(multi_gpu.gpus[0],
		      nus_absolute_path_build("triangle_shader/shader.frag.spv"),
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
    .binding = 0,
    .stride = (unsigned int)NUSM_VERTEX_BYTE_COUNT,
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
  };
  VkVertexInputAttributeDescription vertex_attribute_description[] = {
    {
      .location = 0,
      .binding = vertex_binding_description.binding,
      .format = VK_FORMAT_R32G32B32_SFLOAT,
      .offset = sizeof(float) * 0
    },
    {
      .location = 1,
      .binding = vertex_binding_description.binding,
      .format = VK_FORMAT_R32G32B32_SFLOAT,
      .offset = sizeof(float) * 3
    }
  };
  
  VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .vertexBindingDescriptionCount = 1,
    .pVertexBindingDescriptions = &vertex_binding_description,
    .vertexAttributeDescriptionCount = 2,
    .pVertexAttributeDescriptions = vertex_attribute_description
  };
  
  VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    //.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
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
    .dynamicStateCount = 2,
    .pDynamicStates = dynamics_states
  };
  
  VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .depthClampEnable = VK_FALSE,
    .rasterizerDiscardEnable = VK_FALSE,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_NONE,
    //.cullMode = VK_CULL_MODE_BACK_BIT,
    .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    .depthBiasEnable = VK_FALSE,
    .depthBiasConstantFactor = 0.0f,
    .depthBiasClamp = 0.0f,
    .depthBiasSlopeFactor = 0.0f,
    .lineWidth = 1.0f
  };
  
  VkPipelineMultisampleStateCreateInfo multisample_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_FALSE,
    .minSampleShading = 1.0f,
    .pSampleMask = NULL,
    .alphaToCoverageEnable = VK_FALSE,
    .alphaToOneEnable = VK_FALSE
  };
  
  VkPipelineColorBlendAttachmentState color_blend_attachment_state = {
    .blendEnable = VK_FALSE,
    .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
    .colorBlendOp = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp = VK_BLEND_OP_ADD,
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
  };
  
  VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .logicOpEnable = VK_FALSE,
    .logicOp = VK_LOGIC_OP_COPY,
    .attachmentCount = 1,
    .pAttachments = &color_blend_attachment_state,
    .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
  };

  // Create descriptor set layout
  VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {
    .binding = 0,
    .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    .descriptorCount = 1,
    .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
    .pImmutableSamplers = NULL
  };
  VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .bindingCount = 1,
    .pBindings = &descriptor_set_layout_binding
  };
  VkDescriptorSetLayout descriptor_set_layout;
  if(vkCreateDescriptorSetLayout(present.queue_info.p_gpu->logical_device,
				 &descriptor_set_layout_create_info, NULL,
				 &descriptor_set_layout) != VK_SUCCESS){
    printf("ERROR::failed to create descriptor set layout\n");
    return -1;
  }

  // create descriptor pool
  VkDescriptorPoolSize descriptor_pool_size = {
    .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    .descriptorCount = 1
  };
  VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .maxSets = 1,
    .poolSizeCount = 1,
    .pPoolSizes = &descriptor_pool_size
  };
  VkDescriptorPool descriptor_pool;
  if(vkCreateDescriptorPool(present.queue_info.p_gpu->logical_device,
			    &descriptor_pool_create_info, NULL, &descriptor_pool) !=
     VK_SUCCESS){
    printf("ERROR::failed to create descriptor pool\n");
    return -1;
  }
  
  VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext = NULL,
    .descriptorPool = descriptor_pool,
    .descriptorSetCount = 1,
    .pSetLayouts = &descriptor_set_layout
  };
  VkDescriptorSet descriptor_set;
  if(vkAllocateDescriptorSets(present.queue_info.p_gpu->logical_device,
			      &descriptor_set_allocate_info, &descriptor_set) !=
     VK_SUCCESS){
    printf("ERROR::failed to allocate descriptor set\n");
    return -1;
  }
  
  VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .setLayoutCount = 1,
    .pSetLayouts = &descriptor_set_layout,
    .pushConstantRangeCount = 0,
    .pPushConstantRanges = NULL
  };
  VkPipelineLayout pipeline_layout;
  if(vkCreatePipelineLayout(present.queue_info.p_gpu->logical_device,
			    &pipeline_layout_create_info, NULL,
			    &pipeline_layout) != VK_SUCCESS){
    printf("ERROR::failed to create pipeline layout\n");
    return -1;
  }
  
  VkPipelineDepthStencilStateCreateInfo depth_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .depthTestEnable = VK_TRUE,
    .depthWriteEnable = VK_TRUE,
    .depthCompareOp = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = VK_FALSE,
    .minDepthBounds = 0.0f,
    .maxDepthBounds = 1.0f,
    .stencilTestEnable = VK_FALSE,
    .front = {},
    .back = {}
  };
  
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
    .pDepthStencilState = &depth_create_info,
    .pColorBlendState = &color_blend_state_create_info,
    .pDynamicState = &dynamic_state_create_info,
    .layout = pipeline_layout,
    .renderPass = render_pass,
    .basePipelineHandle = VK_NULL_HANDLE,
    .basePipelineIndex = -1
  };
  
  VkPipeline graphics_pipeline;
  if(vkCreateGraphicsPipelines(present.queue_info.p_gpu->logical_device,
			       VK_NULL_HANDLE, 1, &graphics_pipeline_create_info,
			       NULL, &graphics_pipeline) != VK_SUCCESS){
    printf("ERROR::failed to create graphics pipelines\n");
    return -1;
  }
  
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
  VkClearValue clear_value[] = {
    {
      .color = {{0.4f, 0.1f, 0.3f, 1.0f}},
    },
    {
      .depthStencil = {1.0f, 0}
    }
  };

  VkRenderPassBeginInfo render_pass_begin_info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext = NULL,
    .renderPass = render_pass,
    .framebuffer = framebuffer.vk_framebuffer,
    .renderArea = {
      .offset = {
	.x = 0,
	.y = 0
      },
      .extent = present.swapchain.extent
    },
    .clearValueCount = 2,
    .pClearValues = clear_value
  };
  

  //
  NUS_axes axes = nus_axes_build(nus_vector_build(0.0, 0.0, 1.0),
				 nus_vector_build(0.0, 1.0, 0.0),
				 nus_vector_build(1.0, 0.0, 0.0));
  NUS_vector translation = nus_vector_build(0.0, 0.0, 0.0);
  NUS_matrix tmp = nus_matrix_transformation(translation, axes);
  
  NUS_memory_map uniform_world_memory;
  nus_memory_map_build(present.queue_info, sizeof(NUS_matrix),
		       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &uniform_world_memory);
  nus_memory_map_flush(uniform_world_memory, present.queue_info, &tmp);

  VkDescriptorBufferInfo descriptor_buffer_info = {
    .buffer = uniform_world_memory.buffer,
    .offset = 0,
    .range = VK_WHOLE_SIZE
  };
  VkWriteDescriptorSet write_descriptor_set = {
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .pNext = NULL,
    .dstSet = descriptor_set,
    .dstBinding = 0,
    .dstArrayElement = 0,
    .descriptorCount = 1,
    .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    .pImageInfo = NULL,
    .pBufferInfo = &descriptor_buffer_info,
    .pTexelBufferView = NULL
  };
  
  vkUpdateDescriptorSets(present.queue_info.p_gpu->logical_device, 1,
			 &write_descriptor_set, 0, NULL);
  
  
  VkImageMemoryBarrier barrier_from_undefined_to_present = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext = NULL,
    .srcAccessMask = 0,
    .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    .srcQueueFamilyIndex = info.queue_family_index,
    .dstQueueFamilyIndex = info.queue_family_index,
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
    .srcQueueFamilyIndex = info.queue_family_index,
    .dstQueueFamilyIndex = info.queue_family_index,
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
    .srcQueueFamilyIndex = info.queue_family_index,
    .dstQueueFamilyIndex = info.queue_family_index,
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
  
  nus_queue_info_add_buffer(info, &command_buffer);
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
  vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		    graphics_pipeline);
  
  vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
			  pipeline_layout, 0, 1, &descriptor_set, 0, NULL);
  
  vkCmdSetViewport(command_buffer, 0, 1, &viewport);
  vkCmdSetScissor(command_buffer, 0, 1, &scissor);
  
  VkDeviceSize vertex_memory_offset = 0,
    index_memory_offset = 0;
  vkCmdBindVertexBuffers(command_buffer, 0, 1, &model.vertex_memory.buffer,
			 &vertex_memory_offset);
  vkCmdBindIndexBuffer(command_buffer, model.index_memory.buffer, index_memory_offset,
		       VK_INDEX_TYPE_UINT32);

  vkCmdDrawIndexed(command_buffer, model.contents.index_data_size/4, 1, 0, 0, 0);
  //TODO store NUSM index and vertex counts insted of sizes
  
  vkCmdEndRenderPass(command_buffer);
  
  vkCmdPipelineBarrier(command_buffer,
		       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		       VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
		       0, 0, NULL, 0, NULL, 1, &barrier_from_draw_to_present);
  
  if(vkEndCommandBuffer(command_buffer) != VK_SUCCESS){
    printf("ERROR::Could not record command buffers!\n");
    return NUS_FAILURE;
  }
  
  printf("finished init\n");
  
  // end of temp init code
  
  
  run = 1;
  float x = 0.0f, y = 0.0f, z = 0.4;
  while(run){

    nus_system_events_handle(win);

    
    y += dy;
    x += dx;
    z += dz;
    
    axes = nus_axes_global_pitch(axes, dpitch);
    axes = nus_axes_global_yaw(axes, dyaw);
    translation = nus_vector_build(x, y, z);
    tmp = nus_matrix_transformation(translation, axes);
    tmp = nus_matrix_transpose(tmp);
    //nus_matrix_print(tmp);
    
    nus_memory_map_flush(uniform_world_memory, present.queue_info, &tmp);

    
    if(nus_command_group_add_semaphores(info.p_command_group, 1,
					&present.image_available,
					1, &present.image_presentable) !=
       NUS_SUCCESS){
      printf("ERROR::failed to add semaphores in clear presentation surface\n");
      return NUS_FAILURE;
    }
    nus_command_group_append(info.p_command_group, command_buffer);
    nus_queue_info_submit(info);
    
    if(nus_multi_gpu_submit_commands(multi_gpu) != NUS_SUCCESS){
      printf("ERROR::failed to submit multi gpu command queues\n");
      return -1;
    }
    
    if(nus_presentation_surface_present(&present) != NUS_SUCCESS){
      printf("ERROR::failed to present window\n");
      return -1;
    }
    
  }
  
  printf("freeing unit test %s\n", PROGRAM_NAME);

  vkDeviceWaitIdle(present.queue_info.p_gpu->logical_device);

  nus_memory_map_free(&uniform_world_memory, present.queue_info);

  vkDestroyDescriptorSetLayout(present.queue_info.p_gpu->logical_device,
			       descriptor_set_layout, NULL);
  vkDestroyDescriptorPool(present.queue_info.p_gpu->logical_device,
			  descriptor_pool, NULL);
  
  nus_model_free(info, &model);
  
  nus_shader_free(*present.queue_info.p_gpu, &vertex_shader);
  nus_shader_free(*present.queue_info.p_gpu, &fragment_shader);

  if(pipeline_layout != VK_NULL_HANDLE){
    vkDestroyPipelineLayout(present.queue_info.p_gpu->logical_device,
			    pipeline_layout, NULL);
    pipeline_layout = VK_NULL_HANDLE;
  }
  if(graphics_pipeline != VK_NULL_HANDLE){
    vkDestroyPipeline(present.queue_info.p_gpu->logical_device,
		      graphics_pipeline, NULL);
    graphics_pipeline = VK_NULL_HANDLE;
  }
  if(render_pass != VK_NULL_HANDLE){
    vkDestroyRenderPass(present.queue_info.p_gpu->logical_device,
		      render_pass, NULL);
    render_pass = VK_NULL_HANDLE;
  }
  
  nus_framebuffer_info_free(*present.queue_info.p_gpu, &framebuffer_info);
  nus_framebuffer_free(*present.queue_info.p_gpu, &framebuffer);
  
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

void move_forward(void)
{
  dz -= move_speed;
}
void move_back(void)
{
  dz += move_speed;
}
void move_up(void)
{
  dy -= move_speed;
}
void move_left(void)
{
  dx -= move_speed;
}
void move_down(void)
{
  dy += move_speed;
}
void move_right(void)
{
  dx += move_speed;
}
void pitch_up(void)
{
  dpitch -= rotate_speed;
}
void pitch_down(void)
{
  dpitch += rotate_speed;
}
void yaw_left(void)
{
  dyaw += rotate_speed;
}
void yaw_right(void)
{
  dyaw -= rotate_speed;
}
