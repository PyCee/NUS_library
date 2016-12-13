#include "keyboard.h"
#include <SDL2/SDL.h>
#include <stdio.h>

keyboard gen_keyboard(void)
{
  keyboard keyboard;
  unsigned short i;
  for(i = 0; i < KEYBOARD_NUM_BUTTONS; i++){
    keyboard.is_pressed[i] = 0;
  }
  return keyboard;
}
void keyboard_handle_event(keyboard *keyboard, SDL_Event event)
{
  switch (event.type) {
  case SDL_KEYDOWN:
    if(event.key.keysym.scancode < KEYBOARD_NUM_BUTTONS){
      if(keyboard->is_pressed[event.key.keysym.scancode] == 0){
	keyboard->press_functions[event.key.keysym.scancode]();
	keyboard->is_pressed[event.key.keysym.scancode] = 1;
      }
    }
    break;
  case SDL_KEYUP:
    if(event.key.keysym.scancode < KEYBOARD_NUM_BUTTONS){
      if(keyboard->is_pressed[event.key.keysym.scancode] == 1){
	keyboard->release_functions[event.key.keysym.scancode]();
	keyboard->is_pressed[event.key.keysym.scancode] = 0;
      }
    }
    break;
  default:
    break;
  }
}
