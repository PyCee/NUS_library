#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>
#include "keyboard.h"
#include "mouse.h"
#include "gamepad.h"

typedef struct input{
  void (*exit_function)(void);
  keyboard m_keyboard;
  mouse m_mouse;
  gamepad m_gamepad;
  struct _SDL_GameController **controllers;
  unsigned char *controller_indices,
    num_gamepads;
} input;

input gen_input(void);
void free_input(input *);
void handle_input(input *);
void reset_input(input *);

void set_mouse_motion_function(input *, void(*)(float, float));
void set_mouse_scroll_function(input *, void(*)(float));
void set_mouse_press_function(input *, unsigned char, void(*)(void));
void set_mouse_release_function(input *, unsigned char, void(*)(void));
/* Buttons to be pressed or released: */
/* SDL_BUTTON_LEFT */
/* SDL_BUTTON_MIDDLE */
/* SDL_BUTTON_RIGHT */
/* SDL_BUTTON_X1 */
/* SDL_BUTTON_X2 */

void set_gamepad_axis_function(input *, unsigned char, void(*)(float));
/* Axes to be moved: */
/* SDL_CONTROLLER_AXIS_LEFTX */
/* SDL_CONTROLLER_AXIS_LEFTY */
/* SDL_CONTROLLER_AXIS_RIGHTX */
/* SDL_CONTROLLER_AXIS_RIGHTY */
/* SDL_CONTROLLER_AXIS_TRIGGERLEFT */
/* SDL_CONTROLLER_AXIS_TRIGGERRIGHT  */

void set_gamepad_press_function(input *, unsigned char, void(*)(void));
void set_gamepad_release_function(input *, unsigned char, void(*)(void));
/* Buttons to be pressed or released: */
/* SDL_CONTROLLER_BUTTON_A */
/* SDL_CONTROLLER_BUTTON_B */
/* SDL_CONTROLLER_BUTTON_X */
/* SDL_CONTROLLER_BUTTON_Y */
/* SDL_CONTROLLER_BUTTON_BACK */
/* SDL_CONTROLLER_BUTTON_GUIDE */
/* SDL_CONTROLLER_BUTTON_START */
/* SDL_CONTROLLER_BUTTON_LEFTSTICK */
/* SDL_CONTROLLER_BUTTON_RIGHTSTICK */
/* SDL_CONTROLLER_BUTTON_LEFTSHOULDER */
/* SDL_CONTROLLER_BUTTON_RIGHTSHOULDER */
/* SDL_CONTROLLER_BUTTON_DPAD_UP */
/* SDL_CONTROLLER_BUTTON_DPAD_DOWN */
/* SDL_CONTROLLER_BUTTON_DPAD_LEFT */
/* SDL_CONTROLLER_BUTTON_DPAD_RIGHT */ 

void set_keyboard_press_function(input *, unsigned short, void(*)(void));
void set_keyboard_release_function(input *, unsigned short, void(*)(void));
/* Keyboard Buttons


*/
#endif /* _INPUT_H_ */
