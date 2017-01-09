#include "NUS_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NUS_system_events.h"

NUS_window nus_build_window(char *title, unsigned int width, unsigned int height)
{
  NUS_window NUS_window_;

  NUS_window_.title = malloc(strlen(title) + 1);
  strcpy(NUS_window_.title, title);
  NUS_window_.width = width;
  NUS_window_.height = height;

#ifdef _WIN32

#elif __unix__
  {
    unsigned long black,white;
    
    NUS_window_.display = XOpenDisplay(NULL);
    NUS_window_.screen = DefaultScreen(NUS_window_.display);
    black = BlackPixel(NUS_window_.display, NUS_window_.screen);
    white = WhitePixel(NUS_window_.display, NUS_window_.screen);
    
    NUS_window_.window = XCreateSimpleWindow(NUS_window_.display,
					     DefaultRootWindow(NUS_window_.display),
					     0, 0, width, height, 5, white, black);
    
    XSetStandardProperties(NUS_window_.display, NUS_window_.window, title, title,
			   None, NULL, 0, NULL);
    
    XSelectInput(NUS_window_.display, NUS_window_.window,
		 ExposureMask|ButtonPressMask|ButtonReleaseMask|
		 KeyPressMask|KeyReleaseMask);
    
    NUS_window_.graphics_context = XCreateGC(NUS_window_.display, NUS_window_.window,
					     0, 0);        
    
    XSetBackground(NUS_window_.display, NUS_window_.graphics_context, white);
    XSetForeground(NUS_window_.display, NUS_window_.graphics_context, black);
    
    XClearWindow(NUS_window_.display, NUS_window_.window);
    XMapRaised(NUS_window_.display, NUS_window_.window);
    
    NUS_window_.delete_message = malloc(sizeof(NUS_window_.delete_message));
    *NUS_window_.delete_message = XInternAtom(NUS_window_.display,
					      "WM_DELETE_WINDOW", False);
    XSetWMProtocols(NUS_window_.display, NUS_window_.window,
		    NUS_window_.delete_message, 1);
  }
#endif
  return NUS_window_;
}

void nus_free_window(NUS_window *NUS_window_)
{
  free(NUS_window_->delete_message);
  XFreeGC(NUS_window_->display, NUS_window_->graphics_context);
  XDestroyWindow(NUS_window_->display, NUS_window_->window);
  XCloseDisplay(NUS_window_->display);
  NUS_window_->display = NULL;
  free(NUS_window_->title);
  NUS_window_->title = NULL;
}

void nus_print_window(NUS_window NUS_window_)
{
  printf("NUS_window has title: %s\ndimensions:\nw: %d\nh: %d\n",
	 NUS_window_.title, NUS_window_.width, NUS_window_.height);
}
