#include "NUS_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NUS_window NUS_build_window(char *title, int width, int height)
{
  NUS_window NUS_window_;

  NUS_window_.title = malloc(strlen(title) + 1);
  strcpy(NUS_window_.title, title);
  NUS_window_.width = width;
  NUS_window_.height = height;
  
  #ifdef _WIN32
  printf("creating window for win32\n");

  #elif __unix__
  printf("creating window for unix\n");
  
  NUS_window_.display = XOpenDisplay(NULL);
  NUS_window_.screen = DefaultScreen(NUS_window_.display);

  unsigned long black = BlackPixel(NUS_window_.display, NUS_window_.screen),	
    white = WhitePixel(NUS_window_.display, NUS_window_.screen);
  
  NUS_window_.window = XCreateSimpleWindow(NUS_window_.display,
					   DefaultRootWindow(NUS_window_.display),
					   0, 0, width, height, 5, white, black);
  XSetStandardProperties(NUS_window_.display, NUS_window_.window,
			 NUS_window_.title, NUS_window_.title,
			 None, NULL, 0, NULL);
  
  NUS_window_.graphics_context = XCreateGC(NUS_window_.display, NUS_window_.window, 0,0);
  
  XSetBackground(NUS_window_.display, NUS_window_.graphics_context, white);
  XSetForeground(NUS_window_.display, NUS_window_.graphics_context, black);
  XMapRaised(NUS_window_.display, NUS_window_.window);

  XSelectInput(NUS_window_.display, NUS_window_.window,
	       ButtonPressMask|KeyPressMask|ExposureMask|ButtonPressMask);
  
  #endif
  return NUS_window_;
}

void NUS_free_window(NUS_window *NUS_window_)
{
  free(NUS_window_->title);
  XFreeGC(NUS_window_->display, NUS_window_->graphics_context);
  XDestroyWindow(NUS_window_->display, NUS_window_->window);
  XCloseDisplay(NUS_window_->display);
}

void NUS_print_window(NUS_window NUS_window_)
{
  printf("NUS_window has title: %s\ndimensions:\nw: %d\nh: %d\n",
	 NUS_window_.title, NUS_window_.width, NUS_window_.height);
}
