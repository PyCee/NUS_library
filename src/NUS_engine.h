#ifndef _NUS_ENGINE_H_
#define _NUS_ENGINE_H_

/* Includes files needed for engine use */

#if defined(_WIN32)
#define NUS_OS_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__unix__)
#define NUS_OS_UNIX
#define VK_USE_PLATFORM_XCB_KHR
#endif

/* If any systems that use vulkan are defined enifed ot t*/
#if defined(NUS_SYSTEM_GPU) || defined(NUS_SYSTEM_RENDER)
#define NUS_USE_VULKAN
#endif

/* Include files that should be present in any instance of this engine*/
#include "NUS_result.h"

/*  NUS_SYSTEM_WINDOW must be defined in the application to enable windowing */
#if defined(NUS_SYSTEM_WINDOW)
#include "io/NUS_window.h"
#include "io/NUS_system_events.h"
#endif /* NUS_SYSTEM_WINDOW */

/*  NUS_SYSTEM_GPU must be defined in the application to enable gpu usage */
#if defined(NUS_SYSTEM_GPU)
#include "gpu/NUS_gpu_group.h"
#include "gpu/NUS_vulkan_instance.h"
#endif /* NUS_SYSTEM_GPU */

/*  NUS_SYSTEM_RENDER must be defined in the application to enable rendering */
#if defined(NUS_SYSTEM_RENDER)
#include "render/NUS_presentation_surface.h"
#endif /* NUS_SYSTEM_RENDER */

#if defined(NUS_USE_VULKAN)
#include <vulkan/vulkan.h>
#endif /* NUS_USE_VULKAN */


#endif /* _NUS_ENGINE_H_ */
