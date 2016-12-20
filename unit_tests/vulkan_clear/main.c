#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR /* tells vulkan to use win-api extensions */
#elif __unix__
//XLIB vulkan library not supported by my intel graphics card yet
//#define VK_USE_PLATFORM_XLIB_KHR /* tells vulkan to use X11 extensions */
#define VK_USE_PLATFORM_XCB_KHR /* tells vulkan to use XCB extensions */
#endif

#include <stdio.h>
#include <string.h>
#include <NUS/NUS_window.h>
#include <NUS/NUS_system_events.h>
#include <vulkan/vulkan.h>

#define PROGRAM_NAME "unit_test-vulkan_clear"


#include <X11/Xlib-xcb.h>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <xcb/xcb.h>

char run;

void close_win(void);

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}
  

  NUS_window win = NUS_build_window(PROGRAM_NAME, 600, 400);
  
  NUS_setup_system_events(win);
  NUS_event_handler eve = NUS_build_event_handler();
  eve.close_window = close_win;
  NUS_set_event_handler(&eve);
  run = 1;


  
  unsigned int supported_extension_count = 0;
  if(vkEnumerateInstanceExtensionProperties(NULL, &supported_extension_count, NULL) !=
     VK_SUCCESS || supported_extension_count == 0){
    printf("ERROR::instance extension enumeration\n");
    return -1;
  }
  VkExtensionProperties available_extensions[supported_extension_count];
  if(vkEnumerateInstanceExtensionProperties(NULL, &supported_extension_count,
					    available_extensions) != VK_SUCCESS){
    printf("ERROR::instance extension enumeration secondary\n");
    return -1;
  }
  printf("supported extensions:\n");
  for(unsigned int i = 0; i < supported_extension_count; ++i){
    printf("%s\n", available_extensions[i].extensionName);
  }

  const char *required_extensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
  };

  unsigned int used_extension_count = sizeof(required_extensions) /
    sizeof(*required_extensions);
  
  for(unsigned int i = 0; i < used_extension_count; ++i){
    for(unsigned int j = 0; j < supported_extension_count; ++j){
      if(!strcmp(required_extensions[i], available_extensions[j].extensionName)){
	break;
      }
      if(supported_extension_count - 1 == j){
	printf("ERROR::could not find extension named %s\n", required_extensions[i]);
      }
    }
  }
  
  VkInstance vulkan_instance;
  
  VkApplicationInfo application_info;
  application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  application_info.pApplicationName = PROGRAM_NAME;
  application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  application_info.pEngineName = "NUS";
  application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  application_info.apiVersion = VK_API_VERSION_1_0;
 
  VkInstanceCreateInfo instance_info;
  instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instance_info.pNext = NULL;
  instance_info.flags = 0;
  instance_info.pApplicationInfo = &application_info;
  instance_info.enabledLayerCount = 0;
  instance_info.ppEnabledLayerNames = NULL;
  instance_info.enabledExtensionCount = used_extension_count;
  instance_info.ppEnabledExtensionNames = required_extensions;

  if(vkCreateInstance(&instance_info, NULL, &vulkan_instance) != VK_SUCCESS){
    printf("ERROR::unable to create vulkan instance\n");
    return -1;
  }


  VkSurfaceKHR vulkan_surface;
  
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  VkWin32SurfaceCreateInfoKHR surface_create_info;
  surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  surface_create_info.pNext = NULL;
  surface_create_info.flags = 0;
  //surface_create_info.hinstance = Window.Instance;
  //surface_create_info.hwind = Window.Handle;
  if(vkCreateWin32SurfaceKHR(vulkan_instance, &surface_create_info,
			      NULL, &vulkan_surface) != VK_SUCCESS){
    printf("ERROR::unable to create Win32 vulkan surface\n");
    return -1;
  }
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  
  //must use for unix because my machne doesn't support xlib surface
  VkXcbSurfaceCreateInfoKHR surface_create_info;
  surface_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
  surface_create_info.pNext = NULL;
  surface_create_info.flags = 0;
  surface_create_info.connection = XGetXCBConnection(win.display);
  surface_create_info.window = xcb_generate_id(surface_create_info.connection);
  if(vkCreateXcbSurfaceKHR(vulkan_instance, &surface_create_info,
			   NULL, &vulkan_surface) != VK_SUCCESS){
    printf("ERROR::unable to create XCB vulkan surface\n");
    return -1;
  }
#endif

	   
  VkPhysicalDevice physical_device = VK_NULL_HANDLE;
  unsigned int device_count = 0;
  /* get number of devices */
  vkEnumeratePhysicalDevices(vulkan_instance, &device_count, NULL);
  if(0 == device_count){
    printf("ERROR::no gpu with vulkan support\n");
    return -1;
  }

  VkPhysicalDevice devices[device_count];
  /* get devices */
  vkEnumeratePhysicalDevices(vulkan_instance, &device_count, devices);
  
  unsigned int graphics_oper_family_i = 0;
  
  /* test devices for compatability */
  for(unsigned int i = 0; i < device_count; ++i){
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;
    
    vkGetPhysicalDeviceProperties(devices[i], &device_properties);
    vkGetPhysicalDeviceFeatures(devices[i], &device_features);
    
    unsigned int major_version = VK_VERSION_MAJOR(device_properties.apiVersion);
    unsigned int minor_version = VK_VERSION_MINOR(device_properties.apiVersion);
    unsigned int patch_version = VK_VERSION_PATCH(device_properties.apiVersion);
    if((major_version < 1) &&
	(device_properties.limits.maxImageDimension2D < 4096)){
      /* device doesn't support required parameters */
      printf("physical device %d doesn't support required parameters\n", i);
      continue;
    }
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_family_count, NULL);
    if(queue_family_count == 0) {
      /* device doesn't support any queue families */
      printf("physical device %d doesn't support any queue families\n", i);
      continue;
    }

    VkQueueFamilyProperties queue_family_properties[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_family_count,
					     queue_family_properties);
    for(unsigned int j = 0; j < queue_family_count; ++j){
      if((queue_family_properties[j].queueCount > 0) &&
	 (queue_family_properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)){
	// this queue family is good with graphics operations
	graphics_oper_family_i = j;
      }
    }
    physical_device = devices[i];
  }
  
  if(VK_NULL_HANDLE == physical_device){
    printf("ERROR::no valid device found\n");
    return -1;
  }

  float queue_priorities = 1.0;
  VkDeviceQueueCreateInfo queue_create_info = {
    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    NULL,
    0,
    graphics_oper_family_i,
    1,
    &queue_priorities
  };
  VkDeviceCreateInfo device_create_info = {
    VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    NULL,
    0,
    1,
    &queue_create_info,
    0,
    NULL,
    0,
    NULL,
    NULL
  };

  VkDevice vulkan_device;
  
  if(vkCreateDevice(physical_device, &device_create_info, NULL, &vulkan_device)
     != VK_SUCCESS){
    printf("ERROR::failed to create VkDevice\n");
    return -1;
  }

  VkQueue device_queue;
  /* gets queue of specific queue-family of specific device */
  vkGetDeviceQueue(vulkan_device, graphics_oper_family_i, 0, &device_queue);




  //TODO load and free  vulkan library from device
  while(run){
    NUS_handle_system_events(win);
  }

  // free program
  
  if(VK_NULL_HANDLE != vulkan_device){
    vkDeviceWaitIdle(vulkan_device);
    vkDestroyDevice(vulkan_device, NULL);
  }
  if(VK_NULL_HANDLE != vulkan_instance){
    vkDestroyInstance(vulkan_instance, NULL);
  }
  
  NUS_free_window(&win);
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
void close_win(void){
  run = 0;
}
