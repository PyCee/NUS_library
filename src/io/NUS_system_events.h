#ifndef _NUS_SYSTEM_EVENTS_H_
#define _NUS_SYSTEM_EVENTS_H_

struct NUS_window;

extern void (*quit_function)(void);

// setup callbacks
typedef struct NUS_event_handler{
  void (*key_press)(void);
} NUS_event_handler;

void NUS_handle_system_events(struct NUS_window);

#endif /* NUS_SYSTEM_EVENTS_H_ */
