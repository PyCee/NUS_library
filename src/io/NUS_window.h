#ifndef _NUS_WINDOW_H_
#define _NUS_WINDOW_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct NUS_window{
  char *title;
  int width, height;
  GLFWwindow *window;
} NUS_window;

NUS_window NUS_build_window(char *, int, int);
void NUS_free_window(NUS_window *);
void NUS_print_window(NUS_window);

#endif /* _NUS_WINDOW_H_ */

