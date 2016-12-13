#include "input.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "keyboard.h"
#include "mouse.h"
#include "gamepad.h"


static void useless_button_function(void){}
static void useless_motion_function(float x, float y){}
static void useless_scroll_function(float scroll){}
static void useless_axis_function(float value){}
static void add_input_controller(input *, unsigned char);
static void rem_input_controller(input *, unsigned char);

input gen_input(void)
{
  input input_;
  input_.m_keyboard = gen_keyboard();
  input_.m_mouse = gen_mouse();
  input_.m_gamepad = gen_gamepad();
  input_.controller_indices = NULL;
  input_.controllers = NULL;
  input_.num_gamepads = 0;

  reset_input(&input_);
  
  unsigned char num_joysticks;
  unsigned char i;
  SDL_JoystickEventState(SDL_ENABLE);
  if(SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0)
    if(SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
      printf("ERROR: initializing SDL_INIT_GAMECONTROLLER: %s\n", SDL_GetError());
  if((num_joysticks = SDL_NumJoysticks()) < 0)
    printf("ERROR: obtaining number of game controllers: %s\n", SDL_GetError());
  unsigned char num_gamepads = 0;
  for(i = 0; i < num_joysticks; i++)
    if(SDL_IsGameController(i)) num_gamepads++;
  for(i = 0; i < num_gamepads; i++){
    add_input_controller(&input_, i);
  }
  return input_;
}

void free_input(input *input_)
{
  reset_input(input_);
}
#define KEYBOARD_FLAGS SDL_KEYDOWN | SDL_KEYUP
#define MOUSE_FLAGS SDL_MOUSEMOTION | \
  SDL_MOUSEBUTTONDOWN |	SDL_MOUSEBUTTONUP | SDL_MOUSEWHEEL
#define GAMEPAD_FLAGS SDL_CONTROLLERAXISMOTION | \
  SDL_CONTROLLERBUTTONUP | SDL_CONTROLLERBUTTONDOWN
#define GAMEPAD_CONNECTION SDL_CONTROLLERDEVICEADDED | \
  SDL_CONTROLLERDEVICEREMOVED
void handle_input
(input *input_)
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type){
    case SDL_QUIT:
      input_->exit_function();
    case SDL_KEYUP:
    case SDL_KEYDOWN:
      keyboard_handle_event(&input_->m_keyboard, event);
      break;
    case SDL_MOUSEMOTION:
    case SDL_MOUSEWHEEL:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
      mouse_handle_event(&input_->m_mouse, event);
      break;
    case SDL_CONTROLLERAXISMOTION:
    case SDL_CONTROLLERBUTTONUP:
    case SDL_CONTROLLERBUTTONDOWN:
      gamepad_handle_event(&input_->m_gamepad, event);
      break;
    case SDL_CONTROLLERDEVICEADDED:
      add_input_controller(input_, event.cdevice.which);
      break;
    case SDL_CONTROLLERDEVICEREMOVED:
      rem_input_controller(input_, event.cdevice.which);
      break;
    }
  }
}
void reset_input(input *input_)
{
  unsigned short i;
  for(i = 0; i < KEYBOARD_NUM_BUTTONS; i++){
    set_keyboard_press_function(input_, i, useless_button_function);
    set_keyboard_release_function(input_, i, useless_button_function);
  }
  set_mouse_motion_function(input_, useless_motion_function);
  set_mouse_scroll_function(input_, useless_scroll_function);
  for(i = 0; i < MOUSE_NUM_BUTTONS; i++){
    set_mouse_press_function(input_, i, useless_button_function);
    set_mouse_release_function(input_, i, useless_button_function);
  }
  for(i = 0; i < GAMEPAD_NUM_AXES; i++){
    set_gamepad_axis_function(input_, i, useless_axis_function);
  }
  for(i = 0; i < GAMEPAD_NUM_BUTTONS; i++){
    set_gamepad_press_function(input_, i, useless_button_function);
    set_gamepad_release_function(input_, i, useless_button_function);
  }
}
static void add_input_controller(input *input_, unsigned char device_i)
{
  input_->num_gamepads++;
  size_t con_allosize = sizeof(*input_->controllers) * input_->num_gamepads;
  size_t ind_allosize = sizeof(*input_->controller_indices) * input_->num_gamepads;
  
  if(input_->num_gamepads == 1){
    input_->controllers = malloc(con_allosize);
    input_->controller_indices = malloc(ind_allosize);
  } else {
    input_->controllers = realloc(input_->controllers, con_allosize);
    input_->controller_indices = realloc(input_->controller_indices, ind_allosize);
  }
  input_->controllers[input_->num_gamepads - 1] = SDL_GameControllerOpen(device_i);
  input_->controller_indices[input_->num_gamepads - 1] = device_i;
}
static void rem_input_controller(input *input_, unsigned char device_id)
{
  unsigned char i, con_i;
  SDL_GameController **temp_poi = input_->controllers;
  unsigned char *temp_ind = input_->controller_indices;
  size_t con_size = sizeof(*input_->controllers) * (input_->num_gamepads - 1);
  size_t ind_size = sizeof(*input_->controller_indices) * (input_->num_gamepads - 1);
  if(input_->num_gamepads == 1){
    free(input_->controllers);
    free(input_->controller_indices);
    input_->controllers = NULL;
    input_->controller_indices = NULL;
  }
  else{
    input_->controllers = malloc(con_size);
    input_->controller_indices = malloc(ind_size);
    SDL_GameControllerClose(SDL_GameControllerFromInstanceID(device_id));
    for(i = 0, con_i = 0; i < input_->num_gamepads; i++){
      if(SDL_GameControllerGetAttached(input_->controllers[i]) != SDL_TRUE)
	continue;
      input_->controllers[con_i] = temp_poi[i];
      input_->controller_indices[con_i] = temp_ind[i];
      con_i++;
    }
    free(temp_poi);
  }
  input_->num_gamepads--;
}
void set_mouse_motion_function
(input *input_, void(*function)(float, float))
{
  input_->m_mouse.motion_function = function;
}
void set_mouse_scroll_function
(input *input_, void(*function)(float))
{
  input_->m_mouse.scroll_function = function;
}
void set_mouse_press_function
(input *input_, unsigned char button_index, void(*function)(void))
{
  input_->m_mouse.press_functions[button_index] = function;
}
void set_mouse_release_function
(input *input_, unsigned char button_index, void(*function)(void))
{
  input_->m_mouse.release_functions[button_index] = function;
}
void set_gamepad_axis_function
(input *input_, unsigned char axis_i, void(*function)(float))
{
  input_->m_gamepad.axis_motion_function[axis_i] = function;
}
void set_gamepad_press_function
(input *input_, unsigned char button_i, void(*function)(void))
{
  input_->m_gamepad.press_function[button_i] = function;
}
void set_gamepad_release_function
(input *input_, unsigned char button_i, void(*function)(void))
{
  input_->m_gamepad.release_function[button_i] = function;
}
void set_keyboard_press_function
(input *input_, unsigned short button_i, void(*function)(void))
{
  input_->m_keyboard.press_functions[button_i] = function;
}
void set_keyboard_release_function
(input *input_, unsigned short button_i, void(*function)(void))
{
  input_->m_keyboard.release_functions[button_i] = function;
}
