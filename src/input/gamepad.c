#include "gamepad.h"
#include <stdlib.h>

gamepad gen_gamepad(void)
{
  gamepad gamepad;
  unsigned char i;
  for(i = 0; i < GAMEPAD_NUM_BUTTONS; i++){
    gamepad.is_pressed[i] = 0;
  }
  return gamepad;
}
void gamepad_handle_event(gamepad *gamepad, SDL_Event event)
{
  switch (event.type) {
  case SDL_CONTROLLERAXISMOTION:
    {
      float sign = (event.caxis.value > 0.0) ? 1.0 : -1.0,
	value = sign * event.caxis.value / 32768.0;
      value = (value - GAMEPAD_AXIS_DEADZONE) / (1.0 - GAMEPAD_AXIS_DEADZONE);
      value = (value > 0.0) ? sign * value : 0.0;
      gamepad->axis_motion_function[event.caxis.axis](value);
    }
    break;
  case SDL_CONTROLLERBUTTONDOWN:
    if (event.cbutton.button < GAMEPAD_NUM_BUTTONS){
      if(gamepad->is_pressed[event.cbutton.button] == 0){
	gamepad->press_function[event.cbutton.button]();
	gamepad->is_pressed[event.cbutton.button] = 1;
      }
    }
    break;
  case SDL_CONTROLLERBUTTONUP:
    if (event.cbutton.button < GAMEPAD_NUM_BUTTONS){
      if(gamepad->is_pressed[event.cbutton.button] == 1){
	gamepad->release_function[event.cbutton.button]();
	gamepad->is_pressed[event.cbutton.button] = 0;
      }
    }
    break;
  default:
    break;
  }
}

