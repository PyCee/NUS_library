#include "NUS_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NUS_system_events.h"

NUS_window NUS_build_window(char *title, int width, int height)
{
  NUS_window NUS_window_;

  NUS_window_.title = malloc(strlen(title) + 1);
  strcpy(NUS_window_.title, title);
  NUS_window_.width = width;
  NUS_window_.height = height;

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  NUS_window_. window = glfwCreateWindow(width, height, title, NULL, NULL);
  
  return NUS_window_;
}

void NUS_free_window(NUS_window *NUS_window_)
{
  glfwDestroyWindow(NUS_window_->window);
  NUS_window_->window = NULL;
  free(NUS_window_->title);
  NUS_window_->title = NULL;
}

void NUS_print_window(NUS_window NUS_window_)
{
  printf("NUS_window has title: %s\ndimensions:\nw: %d\nh: %d\n",
	 NUS_window_.title, NUS_window_.width, NUS_window_.height);
}
