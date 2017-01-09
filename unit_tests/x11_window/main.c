#include <stdio.h>

#define NUS_SYSTEM_WINDOW
#include <NUS/NUS_engine.h>

#define PROGRAM_NAME "unit_test-x11_window"

char run;

void close_win(void);

int main(int argc, char *argv[])
{
  NUS_window win;
  NUS_event_handler eve;
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  win = nus_build_window(PROGRAM_NAME, 600, 400);
  nus_setup_system_events(win);
  eve = nus_build_event_handler();
  eve.close_window = close_win;
  nus_set_event_handler(&eve);
  run = 1;
  while(run){
    nus_handle_system_events(win);
  }
  nus_free_window(&win);
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
void close_win(void){
  run = 0;
}
