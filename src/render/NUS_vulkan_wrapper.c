#include "NUS_vulkan_wrapper.h"

void NUS_init_vulkan(void)
{
  /* don't load at runtime, that would require the consumer to have the library installed in a relative directory
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  HMODULE vulkan_library;
  vulkan_library = LoadLibrary("vulkan-1.dll");
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
  void *vulkan_library;
  vulkan_library = dlopen("libvulkan.so", RTLD_NOW);
#endif
  if(vulkan_library == NULL){
    printf("ERROR::could not load vulkan library\n");
  }*/
}
