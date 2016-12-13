#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <NUS_window.h>
#include <NUS_system_events.h>

#define PROGRAM_NAME "unit_test-display_screen"

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  NUS_window win = NUS_build_window(PROGRAM_NAME, 600, 400);

  #ifdef _WIN32
  
  #elif __unix__
  NUS_print_window(win);
  
  
  #endif
  getchar();
  NUS_free_window(&win);
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
