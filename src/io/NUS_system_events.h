#ifndef _NUS_SYSTEM_EVENTS_H_
#define _NUS_SYSTEM_EVENTS_H_

#include "../NUS_result.h"

struct NUS_window;

#define NUS_KEY_COUNT 348
#define NUS_MOUSE_BUTTON_COUNT 3
#define NUS_MOUSE_SCROLL_COUNT 2

#define NUS_DECLARE_EVENT_FUNCTION_GROUP(...)	\
  struct {					\
    void(**functions)( __VA_ARGS__ );		\
    unsigned int function_count;		\
  }

typedef struct NUS_event_handler{
  /* group of callbacks */
  void (*key_press[NUS_KEY_COUNT])(void);
  void (*key_release[NUS_KEY_COUNT])(void);
  void (*mouse_button_press[NUS_MOUSE_BUTTON_COUNT])(void);
  void (*mouse_button_release[NUS_MOUSE_BUTTON_COUNT])(void);
  void (*mouse_scroll[NUS_MOUSE_SCROLL_COUNT])(float);
  void (*mouse_motion)(float, float);
  /*
  struct {
    void (**functions)(void);
    unsigned int num_functions;
  } key_function_group[NUS_NUM_KEYS];
  */
  /*
  NUS_DECLARE_EVENT_FUNCTION_GROUP(void) key_press[NUS_KEY_COUNT];
  NUS_DECLARE_EVENT_FUNCTION_GROUP(void) key_release[NUS_KEY_COUNT];
  NUS_DECLARE_EVENT_FUNCTION_GROUP(void) mouse_button_press[NUS_MOUSE_BUTTON_COUNT];
  */
  void (*close_window)(void);
} NUS_event_handler;

NUS_result nus_event_handler_build(NUS_event_handler *);
void nus_event_handler_set(NUS_event_handler *);
void nus_system_events_handle(struct NUS_window);
void nus_event_handler_add_key(NUS_event_handler *, int, void (*)(void));

#endif /* NUS_SYSTEM_EVENTS_H_ */
