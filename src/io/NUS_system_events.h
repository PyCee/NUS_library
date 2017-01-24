#ifndef _NUS_SYSTEM_EVENTS_H_
#define _NUS_SYSTEM_EVENTS_H_

#include "../NUS_result.h"

struct NUS_window;

#define NUS_NUM_KEYS 348

/*TODO setup callbacks */
typedef struct NUS_event_handler{
  /* group of callbacks */
  void (*key_press[NUS_NUM_KEYS])(void);
  void (*key_release[NUS_NUM_KEYS])(void);
  struct {
    void (**functions)(void);
    unsigned int num_functions;
  } key_function_group[NUS_NUM_KEYS];
  void (*close_window)(void);
} NUS_event_handler;

NUS_result nus_event_handler_build(NUS_event_handler *);
void nus_event_handler_set(NUS_event_handler *);
void nus_system_events_handle(struct NUS_window);
void nus_event_handler_add_key(NUS_event_handler *, int, void (*)(void));

#endif /* NUS_SYSTEM_EVENTS_H_ */
