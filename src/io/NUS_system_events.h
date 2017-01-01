#ifndef _NUS_SYSTEM_EVENTS_H_
#define _NUS_SYSTEM_EVENTS_H_

struct NUS_window;

#define NUS_NUM_KEYS 348

// setup callbacks
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

NUS_event_handler nus_build_event_handler(void);
void nus_set_event_handler(NUS_event_handler *);

void nus_setup_system_events(struct NUS_window);
void nus_handle_system_events(struct NUS_window);
void nus_add_key_function(NUS_event_handler *, int, void (*)(void));

#endif /* NUS_SYSTEM_EVENTS_H_ */
