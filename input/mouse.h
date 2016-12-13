#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <SDL2/SDL.h>

#define MOUSE_NUM_BUTTONS 6

typedef struct mouse{
  void (*motion_function)(float, float);
  void (*scroll_function)(float);
  void (*press_functions[MOUSE_NUM_BUTTONS])(void);
  void (*release_functions[MOUSE_NUM_BUTTONS])(void);
  unsigned char is_pressed[MOUSE_NUM_BUTTONS];
} mouse;

mouse gen_mouse(void);
void reset_mouse(mouse *);
void mouse_handle_event(mouse *, SDL_Event);

#endif /* _MOUSE_H_ */
