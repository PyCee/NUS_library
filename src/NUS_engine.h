#ifndef NUS_ENGINE_H
#define NUS_ENGINE_H

/* Includes files needed for engine use */

/* If any systems that use vulkan are defined */
#if defined(NUS_SYSTEM_GPU) || defined(NUS_SYSTEM_RENDER)
#include "NUS_vulkan.h"
#endif /* NUS_USE_VULKAN */

/* Include files that should be present in any instance of this engine*/
#include "NUS_result.h"

#include "math/NUS_vector.h"
#include "math/NUS_matrix.h"
#include "math/NUS_axes.h"
#include "math/NUS_quaternion.h"
#include "math/NUS_frustum.h"

/*  NUS_SYSTEM_WINDOW must be defined in the application to enable windowing */
#if defined(NUS_SYSTEM_WINDOW)
#include "io/NUS_window.h"
#include "io/NUS_system_events.h"
#endif /* NUS_SYSTEM_WINDOW */

/*  NUS_SYSTEM_GPU must be defined in the application to enable gpu usage */
#if defined(NUS_SYSTEM_GPU)
#include "gpu/NUS_gpu_group.h"
#include "gpu/NUS_gpu.h"
#include "gpu/NUS_queue_family.h"
#include "gpu/NUS_vulkan_instance.h"
#endif /* NUS_SYSTEM_GPU */

/*  NUS_SYSTEM_RENDER must be defined in the application to enable rendering */
#if defined(NUS_SYSTEM_RENDER)
#include "render/NUS_presentation_surface.h"
#endif /* NUS_SYSTEM_RENDER */

#endif /* NUS_ENGINE_H */
