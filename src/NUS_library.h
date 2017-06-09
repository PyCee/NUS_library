#ifndef NUS_LIBRARY_H
#define NUS_LIBRARY_H

/* Includes files needed for engine use */

/* If any systems that use vulkan are defined */
//#if defined(NUS_SYSTEM_GPU) || defined(NUS_SYSTEM_RENDER)
#include "NUS_vulkan.h"
//#endif /* NUS_USE_VULKAN */

/* Include files that should be present in any instance of this engine*/
#include "NUS_result.h"
#include "NUS_save.h"
#include "NUS_log.h"
#include "NUS_key.h"
#include "NUS_bool.h"
#include "NUS_entity.h"

#include "strings/NUS_string_group.h"
#include "strings/NUS_absolute_path.h"

#include "time/NUS_clock.h"

#include "math/NUS_vector.h"
#include "math/NUS_matrix.h"
#include "math/NUS_axes.h"
#include "math/NUS_quaternion.h"
#include "math/NUS_frustum.h"
#include "math/NUS_reference_vector.h"

#include "physics/NUS_kinematic_property.h"
#include "physics/NUS_mass.h"
#include "physics/NUS_physics_state.h"
#include "physics/NUS_movement.h"
#include "physics/NUS_orientation.h"

#include "model/NUS_model.h"
#include "model/NUS_vertex.h"
#include "model/NUS_skeleton.h"
#include "model/NUS_joint.h"
#include "model/NUS_pose_skeleton.h"
#include "model/NUS_pose_joint.h"
#include <NUSM/NUSM_library.h>

/*  NUS_SYSTEM_WINDOW must be defined in the application to enable windowing */
//#if defined(NUS_SYSTEM_WINDOW)
#include "io/NUS_window.h"
#include "io/NUS_system_events.h"
//#endif /* NUS_SYSTEM_WINDOW */

/*  NUS_SYSTEM_GPU must be defined in the application to enable gpu usage */
//#if defined(NUS_SYSTEM_GPU)
#include "gpu/NUS_multi_gpu.h"
#include "gpu/NUS_gpu.h"
#include "gpu/NUS_queue_family.h"
#include "gpu/NUS_command_group.h"
#include "gpu/NUS_vulkan_instance.h"
#include "gpu/NUS_queue_info.h"
#include "gpu/NUS_memory_map.h"
#include "gpu/NUS_single_command.h"
//#endif /* NUS_SYSTEM_GPU */

/*  NUS_SYSTEM_RENDER must be defined in the application to enable rendering */
//#if defined(NUS_SYSTEM_RENDER)
#include "render/NUS_presentation_surface.h"
#include "render/NUS_swapchain.h"
#include "render/NUS_image_clear.h"
#include "render/NUS_shader.h"
#include "render/NUS_texture.h"
#include "render/NUS_subpass_info.h"
#include "render/NUS_render_pass.h"
#include "render/NUS_framebuffer.h"
#include "render/NUS_graphics_pipeline.h"
#include "render/NUS_depth_buffer.h"
//#endif /* NUS_SYSTEM_RENDER */

#endif /* NUS_LIBRARY_H */
