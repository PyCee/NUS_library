#ifndef _NUS_WINDOW_H_
#define _NUS_WINDOW_H_

//TODO include support for apple?
//  xwindows, same as unix
#ifdef _WIN32
//include windows headers
#elif __unix__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#endif

typedef struct NUS_window{
  char *title;
  int width, height;
  #ifdef _WIN32
  
  #elif __unix__
  Display *display;
  int screen;
  Window window;
  GC graphics_context;
  
  #endif
} NUS_window;

NUS_window NUS_build_window(char *, int, int);
void NUS_free_window(NUS_window *);
void NUS_print_window(NUS_window);

#endif /* _NUS_WINDOW_H_ */
