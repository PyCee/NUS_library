
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <NUS/NUS_window.h>
#include <NUS/NUS_system_events.h>

#define PROGRAM_NAME "unit_test-display_screen"

char run;

void close_win(void){
  run = 0;
}

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}
  
  if(!glfwInit()){
    printf("glfw failed to init\n");
    return -1;
  }

  
  NUS_window win = NUS_build_window(PROGRAM_NAME, 600, 400);
  quit_function = close_win;
  run = 1;
  while(run){
    NUS_handle_system_events(win);
  }
  
  NUS_free_window(&win);
  
  glfwTerminate();
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
