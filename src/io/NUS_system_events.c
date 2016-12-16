#include "NUS_system_events.h"
#include "NUS_window.h"
#include <stdio.h>


void (*quit_function)(void);

void NUS_handle_system_events(NUS_window NUS_window_)
{
  printf("handling system events\n");
  quit_function();
}
