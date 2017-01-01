#ifndef _NUS_WINDOW_H_
#define _NUS_WINDOW_H_

#ifdef _WIN32

#elif __unix__
#include <X11/Xlib-xcb.h>

//#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <X11/Xlib-xcb.h>

#endif

typedef struct NUS_window{
  char *title;
  unsigned int width, height;
#ifdef _WIN32

#elif __unix__
  Display *display;
  int screen;
  Window window;
  GC graphics_context;
  Atom *delete_message;
#endif
} NUS_window;

NUS_window nus_build_window(char *, unsigned int, unsigned int);
void nus_free_window(NUS_window *);
void nus_print_window(NUS_window);

#endif /* _NUS_WINDOW_H_ */

