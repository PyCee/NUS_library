#include <stdio.h>
#include <input.h>
int should_live;
void w(void){printf("w\n");}
void a(void){printf("a\n");}
void s(void){printf("s\n");}
void d(void){printf("d\n");}
void ex(void){should_live = 0;}
float g_x, g_y;
void stickx(float x){g_x = x;}
void sticky(float y){g_y = y;}
void main()
{
  g_x = 0.0;
  g_y = 0.0;
  should_live = 1;
  input input_ = gen_input();
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* game_window = SDL_CreateWindow("input library test", 500, 100, 600, 600, SDL_WINDOW_SHOWN);
  set_keyboard_release_function(&input_, SDL_SCANCODE_W, w);
  set_keyboard_release_function(&input_, SDL_SCANCODE_A, a);
  set_keyboard_release_function(&input_, SDL_SCANCODE_S, s);
  set_keyboard_release_function(&input_, SDL_SCANCODE_D, d);  
  input_.exit_function = ex;
  set_gamepad_press_function(&input_, SDL_CONTROLLER_BUTTON_A, a);
  set_mouse_press_function(&input_, SDL_BUTTON_LEFT, d);
  set_gamepad_axis_function(&input_, SDL_CONTROLLER_AXIS_LEFTX, stickx);
  set_gamepad_axis_function(&input_, SDL_CONTROLLER_AXIS_LEFTY, sticky);
  while(should_live){
    handle_input(&input_);
  }
}
