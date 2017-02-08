#ifndef NUS_SYSTEM_EVENTS_H
#define NUS_SYSTEM_EVENTS_H

#include "../NUS_result.h"
#include <stdlib.h>

struct NUS_window;

#define NUS_EVENT_MIN_VALUE 0x1
#define NUS_EVENT_CLOSE_WINDOW (NUS_EVENT_MIN_VALUE)
#define NUS_EVENT_KEY_PRESS (NUS_EVENT_CLOSE_WINDOW + 1)
#define NUS_EVENT_KEY_RELEASE (NUS_EVENT_KEY_PRESS + 1)
#define NUS_EVENT_MOUSE_BUTTON_PRESS (NUS_EVENT_KEY_RELEASE + 1)
#define NUS_EVENT_MOUSE_BUTTON_RELEASE (NUS_EVENT_MOUSE_BUTTON_PRESS + 1)
#define NUS_EVENT_MOUSE_MOTION (NUS_EVENT_MOUSE_BUTTON_RELEASE + 1)
#define NUS_EVENT_MOUSE_SCROLL (NUS_EVENT_MOUSE_MOTION + 1)
#define NUS_EVENT_MAX_VALUE (NUS_EVENT_MOUSE_SCROLL + 1)

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

#define nus_function_group_append(function_group, function)		\
  function_group.functions = realloc(function_group.functions,		\
				     sizeof(*function_group.functions) * \
				     ++function_group.function_count);	\
  function_group.functions[function_group.function_count - 1] = function

#define nus_event_handler_function_append(event_handler,		\
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
void nus_event_handler_set(NUS_event_handler *);
void nus_system_events_handle(struct NUS_window);


#define NUS_DEFINE_KEY_CONVERT(key, value)	\
  const unsigned int NUS_KEY_##key = value

#if defined(NUS_OS_WINDOWS)
//TODO windows keycodes

#elif defined(NUS_OS_UNIX)

#define NUS_MOUSE_BUTTON_LEFT 1
#define NUS_MOUSE_BUTTON_RIGHT 3
#define NUS_MOUSE_BUTTON_MIDDLE 0 //TODO test with actual mmbutton

#define NUS_SCROLL_UP 0
#define NUS_SCROLL_DOWN 1
#define NUS_SCROLL_LEFT 2
#define NUS_SCROLL_RIGHT 3

NUS_DEFINE_KEY_CONVERT(ESC, 9);
NUS_DEFINE_KEY_CONVERT(1, 10);
NUS_DEFINE_KEY_CONVERT(2, 11);
NUS_DEFINE_KEY_CONVERT(3, 12);
NUS_DEFINE_KEY_CONVERT(4, 13);
NUS_DEFINE_KEY_CONVERT(5, 14);
NUS_DEFINE_KEY_CONVERT(6, 15);
NUS_DEFINE_KEY_CONVERT(7, 16);
NUS_DEFINE_KEY_CONVERT(8, 17);
NUS_DEFINE_KEY_CONVERT(9, 18);
NUS_DEFINE_KEY_CONVERT(0, 19);
NUS_DEFINE_KEY_CONVERT(MINUS, 20);
NUS_DEFINE_KEY_CONVERT(EQUALS, 21);
NUS_DEFINE_KEY_CONVERT(BACKSPACE, 22);
NUS_DEFINE_KEY_CONVERT(TAB, 23);
NUS_DEFINE_KEY_CONVERT(Q, 24);
NUS_DEFINE_KEY_CONVERT(W, 25);
NUS_DEFINE_KEY_CONVERT(E, 26);
NUS_DEFINE_KEY_CONVERT(R, 27);
NUS_DEFINE_KEY_CONVERT(T, 28);
NUS_DEFINE_KEY_CONVERT(Y, 29);
NUS_DEFINE_KEY_CONVERT(U, 30);
NUS_DEFINE_KEY_CONVERT(I, 31);
NUS_DEFINE_KEY_CONVERT(O, 32);
NUS_DEFINE_KEY_CONVERT(P, 33);
NUS_DEFINE_KEY_CONVERT(LBRACKET, 34);
NUS_DEFINE_KEY_CONVERT(RBRACKET, 35);
NUS_DEFINE_KEY_CONVERT(ENTER, 36);
NUS_DEFINE_KEY_CONVERT(LCTRL, 37);
NUS_DEFINE_KEY_CONVERT(A, 38);
NUS_DEFINE_KEY_CONVERT(S, 39);
NUS_DEFINE_KEY_CONVERT(D, 40);
NUS_DEFINE_KEY_CONVERT(F, 41);
NUS_DEFINE_KEY_CONVERT(G, 42);
NUS_DEFINE_KEY_CONVERT(H, 43);
NUS_DEFINE_KEY_CONVERT(J, 44);
NUS_DEFINE_KEY_CONVERT(K, 45);
NUS_DEFINE_KEY_CONVERT(L, 46);
NUS_DEFINE_KEY_CONVERT(SEMICOLON, 47);
NUS_DEFINE_KEY_CONVERT(APOSTROPHE, 48);

NUS_DEFINE_KEY_CONVERT(LSHIFT, 50);
NUS_DEFINE_KEY_CONVERT(BACKSLASH, 51);
NUS_DEFINE_KEY_CONVERT(Z, 52);
NUS_DEFINE_KEY_CONVERT(X, 53);
NUS_DEFINE_KEY_CONVERT(C, 54);
NUS_DEFINE_KEY_CONVERT(V, 55);
NUS_DEFINE_KEY_CONVERT(B, 56);
NUS_DEFINE_KEY_CONVERT(N, 57);
NUS_DEFINE_KEY_CONVERT(M, 58);
NUS_DEFINE_KEY_CONVERT(COMMA, 59);
NUS_DEFINE_KEY_CONVERT(PERIOD, 60);
NUS_DEFINE_KEY_CONVERT(RSHIFT, 61);
NUS_DEFINE_KEY_CONVERT(KP_MULTIPLY, 62);
NUS_DEFINE_KEY_CONVERT(LALT, 63);
NUS_DEFINE_KEY_CONVERT(SPACE, 64);

NUS_DEFINE_KEY_CONVERT(NUMLOCK, 77);

NUS_DEFINE_KEY_CONVERT(KP_7, 79);
NUS_DEFINE_KEY_CONVERT(KP_8, 80);
NUS_DEFINE_KEY_CONVERT(KP_9, 81);
NUS_DEFINE_KEY_CONVERT(KP_MINUS, 82);
NUS_DEFINE_KEY_CONVERT(KP_4, 83);
NUS_DEFINE_KEY_CONVERT(KP_5, 84);
NUS_DEFINE_KEY_CONVERT(KP_6, 85);
NUS_DEFINE_KEY_CONVERT(KP_PLUS, 86);
NUS_DEFINE_KEY_CONVERT(KP_1, 87);
NUS_DEFINE_KEY_CONVERT(KP_2, 88);
NUS_DEFINE_KEY_CONVERT(KP_3, 89);
NUS_DEFINE_KEY_CONVERT(KP_0, 90);
NUS_DEFINE_KEY_CONVERT(KP_PERIOD, 91);

NUS_DEFINE_KEY_CONVERT(KP_ENTER, 104);
NUS_DEFINE_KEY_CONVERT(RCTRL, 105);

NUS_DEFINE_KEY_CONVERT(RALT, 108);

NUS_DEFINE_KEY_CONVERT(ARROW_UP, 111);
NUS_DEFINE_KEY_CONVERT(ARROW_LEFT, 113);
NUS_DEFINE_KEY_CONVERT(ARROW_RIGHT, 114);
NUS_DEFINE_KEY_CONVERT(ARROW_DOWN, 116);

#endif /* os-specific key converts */


#endif /* NUS_SYSTEM_EVENTS_H */
