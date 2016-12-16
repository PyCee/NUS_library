#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <SDL2/SDL.h>

#define KEYBOARD_NUM_BUTTONS 255

typedef struct keyboard{
  void (*press_functions[KEYBOARD_NUM_BUTTONS])(void);
  void (*release_functions[KEYBOARD_NUM_BUTTONS])(void);
  unsigned short is_pressed[KEYBOARD_NUM_BUTTONS];
} keyboard;

keyboard gen_keyboard(void);
void reset_keyboard(keyboard *);
void keyboard_handle_event(keyboard *, SDL_Event);

#endif /* _KEYBOARD_H_ */
