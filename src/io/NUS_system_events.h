#ifndef NUS_SYSTEM_EVENTS_H
#define NUS_SYSTEM_EVENTS_H

#include "../NUS_result.h"
#include <stdlib.h>
#include "../NUS_os.h"

struct NUS_window;

typedef enum NUS_event_type{
  NUS_EVENT_MIN_VALUE = 1,
  NUS_EVENT_CLOSE_WINDOW = 2,
  NUS_EVENT_KEY_PRESS = 3,
  NUS_EVENT_KEY_RELEASE = 4,
  NUS_EVENT_MOUSE_BUTTON_PRESS = 5,
  NUS_EVENT_MOUSE_BUTTON_RELEASE = 6,
  NUS_EVENT_MOUSE_MOTION = 7,
  NUS_EVENT_MOUSE_SCROLL = 8,
  NUS_EVENT_MAX_VALUE = 9,
} NUS_event_type;

#define NUS_EVENT_ACTION_PRESS 0x1
#define NUS_EVENT_ACTION_RELEASE 0x2


#define NUS_CLOSE_WINDOW_COUNT 1
#define NUS_KEY_COUNT 348
#define NUS_MOUSE_BUTTON_COUNT 3
#define NUS_MOUSE_MOTION_COUNT 1
#define NUS_MOUSE_SCROLL_COUNT 4

#define NUS_DECLARE_FUNCTION_GROUP(...)		\
  struct {					\
    void (**functions)( __VA_ARGS__ );		\
    unsigned int function_count;		\
  }

#define NUS_FUNCTION_GROUP_INIT(function_group)	\
  function_group.functions = NULL;		\
  function_group.function_count = 0
#define NUS_FUNCTION_GROUP_FREE(function_group)	\
  do{						\
    if(function_group.function_count > 0){	\
      free(function_group.functions);		\
      function_group.functions = 0;		\
    }						\
  }while(0)
    
#define nus_function_group_append(function_group, function)		\
  function_group.functions = realloc(function_group.functions,		\
				     sizeof(*function_group.functions) * \
				     ++function_group.function_count);	\
  function_group.functions[function_group.function_count - 1] = function

#define nus_event_handler_append(event_handler,		\
					  event_type,			\
					  group_index,			\
					  function)			\
  do{									\
    if(event_type >= NUS_EVENT_MAX_VALUE ||				\
       event_type < NUS_EVENT_MIN_VALUE){				\
      printf("ERROR::invalid event specifier passed" #event_type);	\
    }									\
    switch(event_type){							\
    case NUS_EVENT_CLOSE_WINDOW:					\
      nus_function_group_append(event_handler.close_window[group_index], \
				(void*)function);			\
      break;								\
    case NUS_EVENT_KEY_PRESS:						\
      nus_function_group_append(event_handler.key_press[group_index],	\
				(void*)function);			\
      break;								\
    case NUS_EVENT_KEY_RELEASE:						\
      nus_function_group_append(event_handler.key_release[group_index], \
				(void*)function);			\
      break;								\
    case NUS_EVENT_MOUSE_BUTTON_PRESS:					\
      nus_function_group_append(event_handler.mouse_button_press[group_index], \
				(void*)function);			\
      break;								\
    case NUS_EVENT_MOUSE_BUTTON_RELEASE:				\
      nus_function_group_append(event_handler.mouse_button_release[group_index], \
				(void*)function);			\
      break;								\
    case NUS_EVENT_MOUSE_MOTION:					\
      nus_function_group_append(event_handler.mouse_motion[group_index], \
				(void*)function);		\
      break;								\
    case NUS_EVENT_MOUSE_SCROLL:					\
      nus_function_group_append(event_handler.mouse_scroll[group_index], \
				(void*)function);			\
      break;								\
    default:								\
      break;								\
    }									\
  } while(0)

#define nus_function_group_call(function_group, ...)			\
  do{									\
    unsigned int nus_function_counter;					\
    for(nus_function_counter = 0;					\
	nus_function_counter < function_group.function_count;		\
	++nus_function_counter){					\
      function_group.functions[nus_function_counter]( __VA_ARGS__ );	\
    }									\
  } while(0)
      
typedef struct NUS_event_handler{
  /* group of callbacks */
  NUS_DECLARE_FUNCTION_GROUP(void) close_window[NUS_CLOSE_WINDOW_COUNT];
  NUS_DECLARE_FUNCTION_GROUP(void) key_press[NUS_KEY_COUNT];
  NUS_DECLARE_FUNCTION_GROUP(void) key_release[NUS_KEY_COUNT];
  NUS_DECLARE_FUNCTION_GROUP(void) mouse_button_press[NUS_MOUSE_BUTTON_COUNT];
  NUS_DECLARE_FUNCTION_GROUP(void) mouse_button_release[NUS_MOUSE_BUTTON_COUNT];
  NUS_DECLARE_FUNCTION_GROUP(float, float) mouse_motion[NUS_MOUSE_MOTION_COUNT];
  NUS_DECLARE_FUNCTION_GROUP(void) mouse_scroll[NUS_MOUSE_SCROLL_COUNT];
} NUS_event_handler;

NUS_result nus_event_handler_build(NUS_event_handler *);
void nus_event_handler_free(NUS_event_handler *);
void nus_event_handler_set(NUS_event_handler *);
void nus_system_events_handle(struct NUS_window);


#if defined(NUS_OS_WINDOWS)
#define NUS_DEFINE_MOUSE_BUTTON_CONVERT(button, win_value, unix_value)	\
  NUS_MOUSE_BUTTON_##button = win_value,
#define NUS_DEFINE_SCROLL_CONVERT(scroll, win_value, unix_value)	\
  NUS_SCROLL_##scroll = win_value,
#define NUS_DEFINE_KEY_CONVERT(key, win_value, unix_value)	\
  NUS_KEY_##key = win_value,
#elif defined(NUS_OS_UNIX)
#define NUS_DEFINE_MOUSE_BUTTON_CONVERT(button, win_value, unix_value)	\
  NUS_MOUSE_BUTTON_##button = unix_value,
#define NUS_DEFINE_SCROLL_CONVERT(scroll, win_value, unix_value)	\
  NUS_SCROLL_##scroll = unix_value,
#define NUS_DEFINE_KEY_CONVERT(key, win_value, unix_value)	\
  NUS_KEY_##key = unix_value,
#endif


typedef enum NUS_mouse_button_code{
  NUS_DEFINE_MOUSE_BUTTON_CONVERT(LEFT, 0, 1)
  NUS_DEFINE_MOUSE_BUTTON_CONVERT(RIGHT, 0, 3)
  NUS_DEFINE_MOUSE_BUTTON_CONVERT(MIDDLE, 0, 0)//TODO test value with mmbutton
  NUS_MOUSE_BUTTON_NULL = 0
} NUS_mouse_button_code;

typedef enum NUS_scroll_code{
  NUS_DEFINE_SCROLL_CONVERT(UP, 0, 0)
  NUS_DEFINE_SCROLL_CONVERT(DOWN, 0, 1)
  NUS_DEFINE_SCROLL_CONVERT(LEFT, 0, 2)
  NUS_DEFINE_SCROLL_CONVERT(RIGHT, 0, 3)
  NUS_SCROLL_NULL = 0
} NUS_scroll_code;

typedef enum NUS_key_code{
  NUS_DEFINE_KEY_CONVERT(ESC, 0, 9)
  NUS_DEFINE_KEY_CONVERT(1, 0, 10)
  NUS_DEFINE_KEY_CONVERT(2, 0, 11)
  NUS_DEFINE_KEY_CONVERT(3, 0, 12)
  NUS_DEFINE_KEY_CONVERT(4, 0, 13)
  NUS_DEFINE_KEY_CONVERT(5, 0, 14)
  NUS_DEFINE_KEY_CONVERT(6, 0, 15)
  NUS_DEFINE_KEY_CONVERT(7, 0, 16)
  NUS_DEFINE_KEY_CONVERT(8, 0, 17)
  NUS_DEFINE_KEY_CONVERT(9, 0, 18)
  NUS_DEFINE_KEY_CONVERT(0, 0, 19)
  NUS_DEFINE_KEY_CONVERT(MINUS, 0, 20)
  NUS_DEFINE_KEY_CONVERT(EQUALS, 0, 21)
  NUS_DEFINE_KEY_CONVERT(BACKSPACE, 0, 22)
  NUS_DEFINE_KEY_CONVERT(TAB, 0, 23)
  NUS_DEFINE_KEY_CONVERT(Q, 0, 24)
  NUS_DEFINE_KEY_CONVERT(W, 0, 25)
  NUS_DEFINE_KEY_CONVERT(E, 0, 26)
  NUS_DEFINE_KEY_CONVERT(R, 0, 27)
  NUS_DEFINE_KEY_CONVERT(T, 0, 28)
  NUS_DEFINE_KEY_CONVERT(Y, 0, 29)
  NUS_DEFINE_KEY_CONVERT(U, 0, 30)
  NUS_DEFINE_KEY_CONVERT(I, 0, 31)
  NUS_DEFINE_KEY_CONVERT(O, 0, 32)
  NUS_DEFINE_KEY_CONVERT(P, 0, 33)
  NUS_DEFINE_KEY_CONVERT(LBRACKET, 0, 34)
  NUS_DEFINE_KEY_CONVERT(RBRACKET, 0, 35)
  NUS_DEFINE_KEY_CONVERT(ENTER, 0, 36)
  NUS_DEFINE_KEY_CONVERT(LCTRL, 0, 37)
  NUS_DEFINE_KEY_CONVERT(A, 0, 38)
  NUS_DEFINE_KEY_CONVERT(S, 0, 39)
  NUS_DEFINE_KEY_CONVERT(D, 0, 40)
  NUS_DEFINE_KEY_CONVERT(F, 0, 41)
  NUS_DEFINE_KEY_CONVERT(G, 0, 42)
  NUS_DEFINE_KEY_CONVERT(H, 0, 43)
  NUS_DEFINE_KEY_CONVERT(J, 0, 44)
  NUS_DEFINE_KEY_CONVERT(K, 0, 45)
  NUS_DEFINE_KEY_CONVERT(L, 0, 46)
  NUS_DEFINE_KEY_CONVERT(SEMICOLON, 0, 47)
  NUS_DEFINE_KEY_CONVERT(APOSTROPHE, 0, 48)
  
  NUS_DEFINE_KEY_CONVERT(LSHIFT, 0, 50)
  NUS_DEFINE_KEY_CONVERT(BACKSLASH, 0, 51)
  NUS_DEFINE_KEY_CONVERT(Z, 0, 52)
  NUS_DEFINE_KEY_CONVERT(X, 0, 53)
  NUS_DEFINE_KEY_CONVERT(C, 0, 54)
  NUS_DEFINE_KEY_CONVERT(V, 0, 55)
  NUS_DEFINE_KEY_CONVERT(B, 0, 56)
  NUS_DEFINE_KEY_CONVERT(N, 0, 57)
  NUS_DEFINE_KEY_CONVERT(M, 0, 58)
  NUS_DEFINE_KEY_CONVERT(COMMA, 0, 59)
  NUS_DEFINE_KEY_CONVERT(PERIOD, 0, 60)
  NUS_DEFINE_KEY_CONVERT(RSHIFT, 0, 61)
  NUS_DEFINE_KEY_CONVERT(KP_MULTIPLY, 0, 62)
  NUS_DEFINE_KEY_CONVERT(LALT, 0, 63)
  NUS_DEFINE_KEY_CONVERT(SPACE, 0, 64)
  
  NUS_DEFINE_KEY_CONVERT(NUMLOCK, 0, 77)
  
  NUS_DEFINE_KEY_CONVERT(KP_7, 0, 79)
  NUS_DEFINE_KEY_CONVERT(KP_8, 0, 80)
  NUS_DEFINE_KEY_CONVERT(KP_9, 0, 81)
  NUS_DEFINE_KEY_CONVERT(KP_MINUS, 0, 82)
  NUS_DEFINE_KEY_CONVERT(KP_4, 0, 83)
  NUS_DEFINE_KEY_CONVERT(KP_5, 0, 84)
  NUS_DEFINE_KEY_CONVERT(KP_6, 0, 85)
  NUS_DEFINE_KEY_CONVERT(KP_PLUS, 0, 86)
  NUS_DEFINE_KEY_CONVERT(KP_1, 0, 87)
  NUS_DEFINE_KEY_CONVERT(KP_2, 0, 88)
  NUS_DEFINE_KEY_CONVERT(KP_3, 0, 89)
  NUS_DEFINE_KEY_CONVERT(KP_0, 0, 90)
  NUS_DEFINE_KEY_CONVERT(KP_PERIOD, 0, 91)
  
  NUS_DEFINE_KEY_CONVERT(KP_ENTER, 0, 104)
  NUS_DEFINE_KEY_CONVERT(RCTRL, 0, 105)
  
  NUS_DEFINE_KEY_CONVERT(RALT, 0, 108)
  
  NUS_DEFINE_KEY_CONVERT(ARROW_UP, 0, 111)
  NUS_DEFINE_KEY_CONVERT(ARROW_LEFT, 0, 113)
  NUS_DEFINE_KEY_CONVERT(ARROW_RIGHT, 0, 114)
  NUS_DEFINE_KEY_CONVERT(ARROW_DOWN, 0, 116)
  NUS_KEY_NULL = 0
} NUS_key_code;
  
  
#endif /* NUS_SYSTEM_EVENTS_H */
  
