#ifndef NUS_LIBRARY_H
#define NUS_LIBRARY_H

/* Includes files needed for engine use */

/* If any systems that use vulkan are defined */
//#if defined(NUS_SYSTEM_GPU) || defined(NUS_SYSTEM_RENDER)
#include "NUS_vulkan.h"
//#endif /* NUS_USE_VULKAN */

/* Include files that should be present in any instance of this engine*/
#include "NUS_handle.h"
#include "NUS_result.h"
#include "NUS_save.h"
#include "NUS_log.h"
#include "NUS_key.h"
#include "NUS_bool.h"
//#include "NUS_entity.h"
//#include "NUS_population.h"

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

#include "physics/collision/NUS_collidable.h"
#include "physics/collision/NUS_aabb.h"
#include "physics/collision/NUS_collision_info.h"
#include "physics/collision/NUS_collision_type.h"
#include "physics/collision/NUS_rigid_body.h"
#include "physics/collision/NUS_point.h"
#include "physics/collision/NUS_sphere.h"
#include "physics/collision/NUS_box.h"
#include "physics/collision/NUS_capsule.h"

#include "model/NUS_model.h"
#include "model/NUS_vertex.h"
#include "model/NUS_skeleton.h"
#include "model/NUS_keyframe.h"
#include "model/NUS_animation.h"
#include "model/NUS_frame.h"
#include "model/NUS_binary_model.h"

#include "NUSM/NUS_load.h"
#include "NUSM/NUS_store.h"
#include "NUSM/NUS_read.h"
#include "NUSM/NUS_validate.h"

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
#include "gpu/NUS_submission_queue.h"
#include "gpu/NUS_vulkan_instance.h"
#include "gpu/NUS_binding.h"
#include "gpu/NUS_memory_map.h"
#include "gpu/NUS_uniform_buffer.h"
#include "gpu/NUS_single_command.h"
//#endif /* NUS_SYSTEM_GPU */

/*  NUS_SYSTEM_RENDER must be defined in the application to enable rendering */
//#if defined(NUS_SYSTEM_RENDER)
#include "render/NUS_presentation_surface.h"
#include "render/NUS_swapchain.h"
#include "render/NUS_shader.h"
#include "render/NUS_texture.h"
#include "render/NUS_image_view.h"
#include "render/NUS_subpass_info.h"
#include "render/NUS_render_pass.h"
#include "render/NUS_framebuffer.h"
#include "render/NUS_pipeline_layout.h"
#include "render/NUS_graphics_pipeline.h"
#include "render/NUS_depth_buffer.h"
#include "render/NUS_sampler.h"
//#endif /* NUS_SYSTEM_RENDER */

#endif /* NUS_LIBRARY_H */
