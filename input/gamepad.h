#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include <SDL2/SDL.h>

#define GAMEPAD_AXIS_DEADZONE 0.3
#define GAMEPAD_NUM_AXES SDL_CONTROLLER_AXIS_MAX
#define GAMEPAD_NUM_BUTTONS SDL_CONTROLLER_BUTTON_MAX

typedef struct gamepad{
  void (*axis_motion_function[GAMEPAD_NUM_AXES])(float);
  void (*press_function[GAMEPAD_NUM_BUTTONS])(void);
  void (*release_function[GAMEPAD_NUM_BUTTONS])(void);
  unsigned char is_pressed[GAMEPAD_NUM_BUTTONS];
}gamepad;

gamepad gen_gamepad(void);
void reset_gamepad(gamepad *);
void gamepad_handle_event(gamepad *, SDL_Event);

#endif /* _GAMEPAD_H_ */
