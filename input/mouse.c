#include "mouse.h"
#include <SDL2/SDL.h>
#include <stdio.h>

mouse gen_mouse(void)
{
  mouse mouse;
  unsigned char i;
  for(i = 0; i < MOUSE_NUM_BUTTONS; i++){
    mouse.is_pressed[i] = 0;
  }
  return mouse;
}
void mouse_handle_event(mouse *mouse, SDL_Event event)
{
  switch (event.type) {
  case SDL_MOUSEMOTION:
    mouse->motion_function(event.motion.xrel, event.motion.yrel);
    break;
  case SDL_MOUSEWHEEL:
    mouse->scroll_function(event.wheel.y);
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (event.button.button < MOUSE_NUM_BUTTONS){
      if(mouse->is_pressed[event.button.button] == 0){
	mouse->press_functions[event.button.button]();
	mouse->is_pressed[event.button.button] = 1;
      }
    }
    break;
  case SDL_MOUSEBUTTONUP:
    if (event.button.button < MOUSE_NUM_BUTTONS){
      if(mouse->is_pressed[event.button.button] == 1){
	mouse->release_functions[event.button.button]();
	mouse->is_pressed[event.button.button] = 0;
      }
    }
    break;
  default:
    break;
  }
}
