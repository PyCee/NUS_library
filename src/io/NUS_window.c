#include "NUS_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NUS_result nus_window_build
(char *title, unsigned short width, unsigned short height, NUS_window *p_window)
{
  p_window->title = malloc(sizeof(p_window->title) * (strlen(title) + 1));
  strcpy(p_window->title, title);
  p_window->width = width;
  p_window->height = height;

#if defined(NUS_OS_WINDOWS)

#elif defined(NUS_OS_UNIX)
  {
    unsigned int value_mask,
      value_list[2];
    
    p_window->connection = xcb_connect(NULL, NULL);
    if(xcb_connection_has_error(p_window->connection) > 0)
    {
      printf ("ERROR::connecting to window\n");
      return NUS_FAILURE;
    }
    p_window->screen =
      xcb_setup_roots_iterator(xcb_get_setup(p_window->connection)).data;
    p_window->window = xcb_generate_id(p_window->connection);

    value_mask = XCB_CW_EVENT_MASK;
    value_list[0] = XCB_EVENT_MASK_EXPOSURE |
      XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_MOTION |
      XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE;
    
    xcb_create_window(p_window->connection,
		      XCB_COPY_FROM_PARENT,
		      p_window->window,
		      p_window->screen->root,
		      0, 0,
		      width, height,
		      10,
		      XCB_WINDOW_CLASS_INPUT_OUTPUT,
		      p_window->screen->root_visual,
		      value_mask, value_list);
    
    xcb_change_property(p_window->connection, XCB_PROP_MODE_REPLACE,
			p_window->window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
		        (unsigned int)strlen(p_window->title),
			p_window->title); 

    p_window->cookie = xcb_intern_atom(p_window->connection, 1, 12,
					  "WM_PROTOCOLS");
    p_window->reply = xcb_intern_atom_reply(p_window->connection,
					       p_window->cookie, 0);
    p_window->delete_cookie = xcb_intern_atom(p_window->connection, 0, 16,
						 "WM_DELETE_WINDOW");
    p_window->delete_reply = xcb_intern_atom_reply(p_window->connection,
						      p_window->delete_cookie, 0);

    xcb_change_property(p_window->connection, XCB_PROP_MODE_REPLACE,
			p_window->window, (*p_window->reply).atom,
			4, 32, 1, &(*p_window->delete_reply).atom);
    
    xcb_map_window(p_window->connection, p_window->window);
    xcb_flush(p_window->connection);
  }
#endif
  return NUS_SUCCESS;
}

void nus_window_free(NUS_window *p_window)
{
  free(p_window->reply);
  free(p_window->delete_reply);
  
  xcb_disconnect(p_window->connection);
  free(p_window->title);
}

void nus_window_print(NUS_window window)
{
  printf("NUS_window has title: %s\ndimensions:\nw: %d\nh: %d\n",
	 window.title, window.width, window.height);
}
