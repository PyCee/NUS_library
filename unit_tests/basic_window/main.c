#include <stdio.h>

#define NUS_SYSTEM_WINDOW
#include <NUS/NUS_library.h>

#define PROGRAM_NAME "unit_test-basic_window"

char run;

void close_win(void);
void print_mouse_motion(float, float);
void print_scroll_up(void);
void print_scroll_down(void);
void print_scroll_left(void);
void print_scroll_right(void);
void print_left_mb(void);
void print_right_mb(void);
void print_w(void);
void print_a(void);
void print_s(void);
void print_d(void);

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
  
  nus_event_handler_append(eve, NUS_EVENT_CLOSE_WINDOW, 0, close_win);
  nus_event_handler_append(eve, NUS_EVENT_MOUSE_MOTION,
				    0, print_mouse_motion);
  nus_event_handler_append(eve, NUS_EVENT_MOUSE_SCROLL,
				    NUS_SCROLL_UP,
				    print_scroll_up);
  nus_event_handler_append(eve, NUS_EVENT_MOUSE_SCROLL,
				    NUS_SCROLL_DOWN,
				    print_scroll_down);
  nus_event_handler_append(eve, NUS_EVENT_MOUSE_SCROLL,
				    NUS_SCROLL_LEFT,
				    print_scroll_left);
  nus_event_handler_append(eve, NUS_EVENT_MOUSE_SCROLL,
				    NUS_SCROLL_RIGHT,
				    print_scroll_right);
  
  nus_event_handler_append(eve, NUS_EVENT_MOUSE_BUTTON_PRESS,
				    NUS_MOUSE_BUTTON_LEFT, print_left_mb);
  nus_event_handler_append(eve, NUS_EVENT_MOUSE_BUTTON_PRESS,
				    NUS_MOUSE_BUTTON_RIGHT, print_right_mb);
  
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_ESC, close_win);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_W, print_w);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_A, print_a);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_S, print_s);
  nus_event_handler_append(eve, NUS_EVENT_KEY_PRESS, NUS_KEY_D, print_d);
  
  nus_event_handler_set(&eve);

  run = 1;
  while(run){
    nus_system_events_handle(win);
  }

  nus_event_handler_free(&eve);
  nus_window_free(&win);
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
void close_win(void){
  run = 0;
}

void print_mouse_motion(float relative_x, float relative_y)
{
  printf("change in mouse x: %f\nchange in mouse y:%f\n", relative_x, relative_y);
}
void print_scroll_up(void)
{
  printf("scrolled up\n");
}
void print_scroll_down(void)
{
  printf("scrolled down\n");
}
void print_scroll_left(void)
{
  printf("scrolled left\n");
}
void print_scroll_right(void)
{
  printf("scrolled right\n");
}
void print_left_mb(void)
{
  printf("left click\n");
}
void print_right_mb(void)
{
  printf("right click\n");
}
void print_w(void)
{
  printf("w\n");
}
void print_a(void)
{
  printf("a\n");
}
void print_s(void)
{
  printf("s\n");
}
void print_d(void)
{
  printf("d\n");
}
