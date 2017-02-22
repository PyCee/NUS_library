
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUS_SYSTEM_GPU
#define NUS_SYSTEM_WINDOW
#define NUS_SYSTEM_RENDER
#include <NUS/NUS_library.h>
#include <limits.h>

#include <unistd.h>

#define PROGRAM_NAME "unit_test-hello_triangle"

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
  nus_event_handler_function_append(eve, NUS_EVENT_CLOSE_WINDOW, 0, close_win);
  
  nus_event_handler_function_append(eve, NUS_EVENT_KEY_PRESS,
				    NUS_KEY_ESC, close_win);
  nus_event_handler_set(&eve);
  
  NUS_vulkan_instance vulkan_instance;
  NUS_string_group extensions;
  nus_string_group_build(extensions, 
			 VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(NUS_OS_WINDOWS)
			 VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(NUS_OS_UNIX)
			 VK_KHR_XCB_SURFACE_EXTENSION_NAME
#endif
			 );
  NUS_string_group layers;
  nus_string_group_init(&layers);
  
  if(nus_vulkan_instance_build(&vulkan_instance, extensions, layers) !=
     NUS_SUCCESS){
    printf("ERROR::failed to create vulkan instance info\n");
    return -1;
  }

  nus_string_group_free(&extensions);
  nus_string_group_free(&layers);
  
  
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
  
  
  VkAttachmentDescription attachment_descriptions[] = {
    {
      .flags = 0,
      .format = present.format.format,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
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
  
  VkRenderPassCreateInfo render_pass_create_info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .attachmentCount = 1,
    .pAttachments = attachment_descriptions,
    .subpassCount = 1,
    .pSubpasses = subpass_descriptions,
    .dependencyCount = 0,
    .pDependencies = NULL
  };

  VkRenderPass render_pass;
  if(vkCreateRenderPass(present.presenting_gpu->logical_device,
			&render_pass_create_info, NULL,
			&render_pass) != VK_SUCCESS){
    printf("ERROR::failed to create render pass\n");
    return -1;
  }
  
  
  VkImageViewCreateInfo image_view_create_info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .image = present.render_image,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = present.format.format,
    .components = {
      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      .a = VK_COMPONENT_SWIZZLE_IDENTITY
    },
    .subresourceRange = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1
    }
  };

  VkImageView image_view;
  
  if(vkCreateImageView(present.presenting_gpu->logical_device,
		       &image_view_create_info, NULL,
		       &image_view) != VK_SUCCESS){
    printf("ERROR::failed to create image view\n");
    return -1;
  }
  
  VkFramebufferCreateInfo framebuffer_create_info = {
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .renderPass = render_pass,
    .attachmentCount = 1,
    .pAttachments = &image_view,
    .width = 600,
    .height = 400,
    .layers = 1
  };
  
  VkFramebuffer framebuffer;
  
  if(vkCreateFramebuffer(present.presenting_gpu->logical_device,
			 &framebuffer_create_info, NULL,
			 &framebuffer) != VK_SUCCESS){
    printf("ERROR::failed to create framebuffer\n");
    return -1;
  }
  
  
  NUS_shader vertex_shader,
    fragment_shader;
  if(nus_shader_build(multi_gpu.gpus[0], "triangle_shader/vert.spv",
		      &vertex_shader) != NUS_SUCCESS){
    printf("ERROR::failed to build vertex shader\n");
    return -1;
  }
  if(nus_shader_build(multi_gpu.gpus[0], "triangle_shader/frag.spv",
		      &fragment_shader) != NUS_SUCCESS){
    printf("ERROR::failed to build fragment shader\n");
    return -1;
  }
  
  VkPipelineShaderStageCreateInfo shader_stage_create_info[] = {
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .stage = VK_SHADER_STAGE_VERTEX_BIT,
      .module = vertex_shader.module,
      .pName = "main",
      .pSpecializationInfo = NULL
    },
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = fragment_shader.module,
      .pName = "main",
      .pSpecializationInfo = NULL
    }
  };
  
  VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .vertexBindingDescriptionCount = 0,
    .pVertexBindingDescriptions = NULL,
    .vertexAttributeDescriptionCount = 0,
    .pVertexAttributeDescriptions = NULL
  };
  
  VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    .primitiveRestartEnable = VK_FALSE
  };
  
  VkViewport viewport = {
    .x = 0,
    .y = 0,
    .width = 600,
    .height = 400,
    .minDepth = 0.0,
    .maxDepth = 1.0
  };
  VkRect2D scissor = {
    .offset = {
      .x = 0,
      .y = 0
    },
    .extent = {
      .width = 600,
      .height = 400
    }
  };
  
  VkPipelineViewportStateCreateInfo viewport_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .viewportCount = 1,
    .pViewports = &viewport,
    .scissorCount = 1,
    .pScissors = &scissor
  };
  
  VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .depthClampEnable = VK_FALSE,
    .rasterizerDiscardEnable = VK_FALSE,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_BACK_BIT,
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
  
  VkPipelineLayoutCreateInfo layout_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .setLayoutCount = 0,
    .pSetLayouts = NULL,
    .pushConstantRangeCount = 0,
    .pPushConstantRanges = NULL
  };
  VkPipelineLayout pipeline_layout;
  if(vkCreatePipelineLayout(present.presenting_gpu->logical_device,
			    &layout_create_info, NULL,
			    &pipeline_layout) != VK_SUCCESS){
    printf("ERROR::failed to create pipeline layout\n");
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
    .pDynamicState = NULL,
    .layout = pipeline_layout,
    .renderPass = render_pass,
    .basePipelineHandle = VK_NULL_HANDLE,
    .basePipelineIndex = -1
  };
  printf("pre create graphics pipeline\n");
  VkPipeline graphics_pipeline;
  
  if(vkCreateGraphicsPipelines(present.presenting_gpu->logical_device,
			       VK_NULL_HANDLE, 1, &graphics_pipeline_create_info,
			       NULL, &graphics_pipeline) != VK_SUCCESS){
    printf("ERROR::failed to create graphics pipelines\n");
    return -1;
  }
  printf("post create graphics pipeline\n");
  
  
  printf("finished init\n");
  // end of temp init code
  
  run = 1;
  while(run){
    nus_system_events_handle(win);

    // temp loop code






    // end of temp loop code
    
    
    //TODO: fix
    //  if I change the clear color at runtime, the window flickers black
    //    this only happens If i updateat a certain speed
    /*
    if(nus_image_clear(present.image_available,
		       present.image_rendered,
		       (VkClearColorValue){{0.0f, 0.0f, 0.0f, 0.0f}},
		       multi_gpu.gpus[0], present.render_image) !=
       NUS_SUCCESS){
      printf("ERROR::failed to clear window\n");
      return -1;
    }
    if(nus_multi_gpu_submit_commands(multi_gpu) != NUS_SUCCESS){
      printf("ERROR::failed to submit multi gpu command queues\n");
      return -1;
    }
    if(nus_presentation_surface_present(&present) != NUS_SUCCESS){
      printf("ERROR::failed to present window\n");
      return -1;
    }
    */
  }
  
  printf("freeing unit test %s\n", PROGRAM_NAME);
   
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
