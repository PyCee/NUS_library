#include "NUS_system_events.h"
#include "NUS_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static NUS_event_handler *NUS_curr_event_handler = NULL;

static void nus_nothing_void(void){}
//static void nus_nothing_float(float float_){}

static void nus_close_window_callback(void);
static void nus_key_callback(unsigned int, unsigned char);

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
  unsigned int i,
    j;
  xcb_generic_event_t *event = NULL,
    **event_list = NULL,
    **tmp_event_list = NULL;
  unsigned int event_count = 0,
    tmp_event_count = 0;
  
  while((event = xcb_poll_for_event(NUS_window_.connection))){
    event_list = realloc(event_list, sizeof(*event_list) * ++event_count);
    event_list[event_count - 1] = event;
  }
  
  tmp_event_count = event_count;
  for(i = 0, j = 0; i < event_count; ++i){
    if((i < event_count - 1) &&
       ((event_list[i]->response_type & ~0x80) == XCB_KEY_RELEASE) &&
       ((event_list[i+1]->response_type & ~0x80) == XCB_KEY_PRESS) &&
       (((xcb_key_release_event_t*)event_list[i])->time ==
	((xcb_key_press_event_t*)event_list[i + 1])->time)){
      // If the next 2 events are the result of holding a key down
      free(event_list[i]);
      event_list[i] = NULL;
      free(event_list[i + 1]);
      event_list[i + 1] = NULL;
      tmp_event_count -= 2;
      ++i;
    } else{
      // Else append the event
      tmp_event_list = realloc(tmp_event_list, sizeof(*tmp_event_list) * (j + 1));
      tmp_event_list[j] = event_list[i];
      j++;
    }
  }
  event_count = tmp_event_count;
  free(event_list);
  event_list = tmp_event_list;
  tmp_event_list = NULL;

  for(i = 0; i < event_count; ++i){
    event = event_list[i];
    switch(event->response_type & ~0x80){
    case XCB_CLIENT_MESSAGE:
      printf("Client Message\n");
      if(((xcb_client_message_event_t*)event)->data.data32[0] ==
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
      //print char from detail
      //be able to get detail from keysym
      printf("key_pressed %d\n", ((*(xcb_key_press_event_t*)event)).detail);
      break;
    case XCB_KEY_RELEASE:
      printf("key released\n");
      break;
    default:
      break;
    }
    free(event);
  }
  free(event_list);
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
static void nus_key_callback(unsigned int key, unsigned char action)
{/*TODO: have parameters that describe: key, press/release*/
  assert(NUS_curr_event_handler);
  /*for(int i = 0; i < NUS_curr_event_handler->key_function_group[].num_functions; ++i){
    NUS_curr_event_handler->key_function_group[].functions[i]();
  }*/
  printf("keycode is %d\n", key);
}
