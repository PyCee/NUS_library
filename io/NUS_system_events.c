#include "NUS_system_events.h"
#include "NUS_window.h"

void NUS_handle_system_events(NUS_window NUS_window_)
{
  XKeyEvent event;
  while(XPending(NUS_window_.display)){
    XNextEvent(NUS_window_.display, (XEvent *)&event);
    switch(event.type){
    case Expose:
      break;
    case KeyRelease:
    case KeyPress:
      //TODO handle user input
      break;
    default:
      break;
    }
  }
}
