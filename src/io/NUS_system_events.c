#include "NUS_system_events.h"
#include "NUS_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static NUS_event_handler *NUS_curr_event_handler = NULL;

static void nus_nothing_void(void){}
//static void nus_nothing_float(float float_){}

static void nus_close_window_callback(void);
static void nus_key_callback(unsigned int);

NUS_event_handler nus_build_event_handler(void)
{
  NUS_event_handler NUS_event_handler_;
  short i;
  for(i = 0; i < NUS_NUM_KEYS; i++){
    NUS_event_handler_.key_press[i] = nus_nothing_void;
    NUS_event_handler_.key_release[i] = nus_nothing_void;
  }
  NUS_event_handler_.close_window = nus_nothing_void;
  return NUS_event_handler_;
}
void nus_set_event_handler(NUS_event_handler *NUS_event_handler_)
{
  NUS_curr_event_handler = NUS_event_handler_;
}
void nus_setup_system_events(NUS_window NUS_window_)
{
  
  /*TODO set calbacks for:
    -mouse movement
    -mouse buttons
    -joystick axes
    -joystick buttons

   */
}
void nus_handle_system_events(NUS_window NUS_window_)
{
  XEvent x_event;
  while(XPending(NUS_window_.display)){
    XNextEvent(NUS_window_.display, &x_event);
    switch(x_event.type){
    case ButtonPress:
      printf("button pressed\n");
      break;
    case ButtonRelease:
      printf("button released\n");
      break;
    case KeyPress:
      nus_key_callback(x_event.xkey.keycode);
      break;
    case KeyRelease:

      /* erases repeat press/release events that occur when a key is held down */
      if(XPending(NUS_window_.display)){
	XEvent next_event;
	XPeekEvent(NUS_window_.display, &next_event);
	if(next_event.type == KeyPress && next_event.xkey.time == x_event.xkey.time &&
	   next_event.xkey.keycode == x_event.xkey.keycode){
	  XNextEvent(NUS_window_.display, &next_event);
	  break;
	}
      }
      
      nus_key_callback(x_event.xkey.keycode);
      break;
    case ClientMessage:
      if(x_event.xclient.data.l[0] == *NUS_window_.delete_message)
	nus_close_window_callback();
      break;
    default:
      break;
    }
  }
}

void nus_add_key_function
(NUS_event_handler *NUS_event_handler_, int key, void (*key_function)(void))
{
  NUS_event_handler_->key_function_group[key].functions =
    realloc(NUS_event_handler_->key_function_group[key].functions,
	    sizeof(void*) * ++NUS_event_handler_->key_function_group[key].num_functions);
  NUS_event_handler_->key_function_group[key].functions
    [NUS_event_handler_->key_function_group[key].num_functions - 1] = key_function;
}
static void nus_close_window_callback(void)
{
  assert(NUS_curr_event_handler);
  NUS_curr_event_handler->close_window();
}
static void nus_key_callback(unsigned int key)
{/*TODO: have parameters that describe: key, press/release*/
  assert(NUS_curr_event_handler);
  /*for(int i = 0; i < NUS_curr_event_handler->key_function_group[].num_functions; ++i){
    NUS_curr_event_handler->key_function_group[].functions[i]();
  }*/
  printf("keycode is %d\n", key);
}
