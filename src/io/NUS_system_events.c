#include "NUS_system_events.h"
#include "NUS_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static NUS_event_handler *NUS_bound_event_handler = NULL;

static void nus_callback_close_window(void);
static void nus_callback_key(unsigned int, unsigned char);
static void nus_callback_mouse_button(int, unsigned char);
static void nus_callback_mouse_scroll(int);
static void nus_callback_mouse_motion(float, float);

NUS_result nus_event_handler_build(NUS_event_handler *NUS_event_handler_)
{
  short i;
  for(i = 0; i < NUS_CLOSE_WINDOW_COUNT; ++i){
    NUS_FUNCTION_GROUP_INIT(NUS_event_handler_->close_window[i]);
  }
  for(i = 0; i < NUS_KEY_COUNT; i++){
    NUS_FUNCTION_GROUP_INIT(NUS_event_handler_->key_press[i]);
    NUS_FUNCTION_GROUP_INIT(NUS_event_handler_->key_release[i]);
  }
  for(i = 0; i < NUS_MOUSE_BUTTON_COUNT; i++){
    NUS_FUNCTION_GROUP_INIT(NUS_event_handler_->mouse_button_press[i]);
    NUS_FUNCTION_GROUP_INIT(NUS_event_handler_->mouse_button_release[i]);
  }
  for(i = 0; i < NUS_MOUSE_SCROLL_COUNT; i++){
    NUS_FUNCTION_GROUP_INIT(NUS_event_handler_->mouse_scroll[i]);
  }
  for(i = 0; i < NUS_MOUSE_MOTION_COUNT; i++){
    NUS_FUNCTION_GROUP_INIT(NUS_event_handler_->mouse_motion[i]);
  }
  return NUS_SUCCESS;
}
void nus_event_handler_set(NUS_event_handler *NUS_event_handler_)
{
  NUS_bound_event_handler = NUS_event_handler_;
}
void nus_system_events_handle(NUS_window NUS_window_)
{
  static float mouse_position_x = 0.0,
    mouse_position_y = 0.0;
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

  if(event_count > 0){
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
      i += 1; // Skip the next event
    } else{
      // Else append the event
      tmp_event_list = realloc(tmp_event_list, sizeof(*tmp_event_list) * (j + 1));
      tmp_event_list[j] = event_list[i];
      j++;
      if(((event_list[i]->response_type & ~0x80) == XCB_BUTTON_RELEASE) &&
	 ((((xcb_button_release_event_t*)event_list[i])->detail >= 4) &&
	  (((xcb_button_release_event_t*)event_list[i])->detail <= 7))){
	// If event is a scroll release (meaningless), ignore
	free(event_list[i]);
	event_list[i] = NULL;
	tmp_event_count -= 1;
      }
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
      if(((xcb_client_message_event_t*)event)->data.data32[0] ==
	   (*NUS_window_.delete_reply).atom){
	  nus_callback_close_window();
        }
        break;
    case XCB_EXPOSE:
      break;
    case XCB_KEY_PRESS:
      nus_callback_key(((*(xcb_key_press_event_t*)event)).detail,
		       NUS_EVENT_ACTION_PRESS);
      break;
    case XCB_KEY_RELEASE:
      nus_callback_key(((*(xcb_key_release_event_t*)event)).detail,
		       NUS_EVENT_ACTION_RELEASE);
      break;
    case XCB_BUTTON_PRESS:
      if((((xcb_button_release_event_t*)event_list[i])->detail >= 4) &&
	 (((xcb_button_release_event_t*)event_list[i])->detail <= 7)){
	// If button press is actually a scroll
	nus_callback_mouse_scroll(((xcb_button_release_event_t*)
				   event_list[i])->detail - 4);
      } else{
	nus_callback_mouse_button(((xcb_button_release_event_t*)
				   event_list[i])->detail,
				  NUS_EVENT_ACTION_PRESS);
      }
      break;
    case XCB_BUTTON_RELEASE:
      nus_callback_mouse_button(((xcb_button_release_event_t*)
				 event_list[i])->detail,
				NUS_EVENT_ACTION_RELEASE);
      break;
    case XCB_MOTION_NOTIFY:
      nus_callback_mouse_motion(((xcb_motion_notify_event_t*)
				 event_list[i])->event_x - mouse_position_x,
				((xcb_motion_notify_event_t*)
				 event_list[i])->event_y - mouse_position_y);
      mouse_position_x = ((xcb_motion_notify_event_t*)event_list[i])->event_x;
      mouse_position_y = ((xcb_motion_notify_event_t*)event_list[i])->event_y;
      break;
    default:
      break;
    }
    free(event);
  }
  free(event_list);
#endif
}

static void nus_callback_close_window(void)
{
  assert(NUS_bound_event_handler);
  unsigned int i;
  for(i = 0; i < NUS_EVENT_CLOSE_WINDOW; ++i){
    nus_function_group_call(NUS_bound_event_handler->close_window[i]);
  }
}
static void nus_callback_key(unsigned int key, unsigned char action)
{
  assert(NUS_bound_event_handler);
  if(key >= NUS_KEY_COUNT){
    printf("ERROR::detected key event with value greater than NUS_KEY_COUNT\n");
    return;
  }
  if(NUS_EVENT_ACTION_PRESS == action){
    nus_function_group_call(NUS_bound_event_handler->key_press[key]);
  } else if(NUS_EVENT_ACTION_RELEASE == action){
    nus_function_group_call(NUS_bound_event_handler->key_release[key]);
  } else{
    printf("ERROR::key callback event with unknown action\n");
  }
}
static void nus_callback_mouse_button(int button, unsigned char action)
{
  assert(NUS_bound_event_handler);
  if(button > NUS_MOUSE_BUTTON_COUNT || button < 0){
    printf("ERROR::detected mouse button event \
with value not in bounds\n");
    return;
    }
  if(NUS_EVENT_ACTION_PRESS == action){
    nus_function_group_call(NUS_bound_event_handler->mouse_button_press[button]);
  } else if(NUS_EVENT_ACTION_RELEASE == action){
    nus_function_group_call(NUS_bound_event_handler->mouse_button_release[button]);
  } else{
    printf("ERROR::mouse button callback with unknown action\n");
  }
}
static void nus_callback_mouse_scroll(int scroll)
{
  assert(NUS_bound_event_handler);
  nus_function_group_call(NUS_bound_event_handler->mouse_scroll[scroll]);
}
static void nus_callback_mouse_motion(float relative_x, float relative_y)
{
  assert(NUS_bound_event_handler);
  nus_function_group_call(NUS_bound_event_handler->mouse_motion[0],
			  relative_x, relative_y);
}
