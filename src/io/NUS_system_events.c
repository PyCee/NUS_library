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

NUS_result nus_event_handler_build(NUS_event_handler *NUS_event_handler_)
{
  short i;
  for(i = 0; i < NUS_KEY_COUNT; i++){
    NUS_event_handler_->key_press[i] = nus_nothing_void;
    NUS_event_handler_->key_release[i] = nus_nothing_void;
  }
  NUS_event_handler_->close_window = nus_nothing_void;
  return NUS_SUCCESS;
}
void nus_event_handler_set(NUS_event_handler *NUS_event_handler_)
{
  NUS_curr_event_handler = NUS_event_handler_;
}
void nus_system_events_handle(NUS_window NUS_window_)
{
#if defined(NUS_OS_WINDOWS)
#elif defined(NUS_OS_UNIX)
  xcb_generic_event_t *event;
  while ((event = xcb_poll_for_event (NUS_window_.connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_CLIENT_MESSAGE:
      printf("Client Message\n");
        if((*(xcb_client_message_event_t*)event).data.data32[0] ==
	   (*NUS_window_.delete_reply).atom){
	  nus_close_window_callback();
          printf("Kill client\n");
        }
        break;
    case XCB_EXPOSE:
      break;
    case XCB_BUTTON_PRESS:
      printf("button_pressed\n");
      break;
    case XCB_BUTTON_RELEASE:
      printf("button released\n");
      break;
    case XCB_KEY_PRESS:
      printf("key_pressed\n");
      break;
    case XCB_KEY_RELEASE:
      printf("key released\n");
      break;
    default:
      break;
    }
    free(event);
  }
#endif
}

void nus_event_handler_add_key
(NUS_event_handler *NUS_event_handler_, int key, void (*key_function)(void))
{
  //impliment function groups
  /*NUS_event_handler_->key_function_group[key].functions =
    realloc(NUS_event_handler_->key_function_group[key].functions,
	    sizeof(void*) * ++NUS_event_handler_->key_function_group[key].num_functions);
  NUS_event_handler_->key_function_group[key].functions
    [NUS_event_handler_->key_function_group[key].num_functions - 1] = key_function;
  */
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
