#include <stdio.h>

#define NUS_SYSTEM_WINDOW
#include <NUS/NUS_library.h>

#define PROGRAM_NAME "unit_test-xcb_window"

char run;

void close_win(void);

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  NUS_window win;
  NUS_event_handler eve;
  
  if(nus_window_build(PROGRAM_NAME, 600, 400, &win) != NUS_SUCCESS){
    printf("ERROR::failed to build nus window\n");
    return -1;
  }
  if(nus_event_handler_build(&eve) != NUS_SUCCESS){
    printf("ERROR::failed to build event handler\n");
    return -1;
  }
  eve.close_window = close_win;
  nus_event_handler_set(&eve);
  run = 1;
  while(run){
    nus_system_events_handle(win);
  }
  
  nus_window_free(&win);
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
void close_win(void){
  run = 0;
}
