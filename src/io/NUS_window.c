#include "NUS_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NUS_system_events.h"

NUS_result nus_window_build
(char *title, unsigned short width, unsigned short height, NUS_window *NUS_window_)
{
  NUS_window_->title = malloc(strlen(title) + 1);
  strcpy(NUS_window_->title, title);
  NUS_window_->width = width;
  NUS_window_->height = height;

#if defined(NUS_OS_WINDOWS)

#elif defined(NUS_OS_UNIX)
  {
    unsigned int value_mask,
      value_list[2];
    
    NUS_window_->connection = xcb_connect(NULL, NULL);
    if(xcb_connection_has_error(NUS_window_->connection) > 0)
    {
      printf ("ERROR::connecting to window\n");
      return NUS_FAILURE;
    }
    NUS_window_->screen =
      xcb_setup_roots_iterator(xcb_get_setup(NUS_window_->connection)).data;
    NUS_window_->window = xcb_generate_id(NUS_window_->connection);

    value_mask = XCB_CW_EVENT_MASK;
    value_list[0] = XCB_EVENT_MASK_EXPOSURE |
      XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_MOTION |
      XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE;
    
    xcb_create_window(NUS_window_->connection,
		      XCB_COPY_FROM_PARENT,
		      NUS_window_->window,
		      NUS_window_->screen->root,
		      0, 0,
		      width, height,
		      10,
		      XCB_WINDOW_CLASS_INPUT_OUTPUT,
		      NUS_window_->screen->root_visual,
		      value_mask, value_list);
    
    xcb_change_property(NUS_window_->connection, XCB_PROP_MODE_REPLACE,
			NUS_window_->window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
		        strlen(NUS_window_->title),
			NUS_window_->title); 

    NUS_window_->cookie = xcb_intern_atom(NUS_window_->connection, 1, 12,
					  "WM_PROTOCOLS");
    NUS_window_->reply = xcb_intern_atom_reply(NUS_window_->connection,
					       NUS_window_->cookie, 0);
    NUS_window_->delete_cookie = xcb_intern_atom(NUS_window_->connection, 0, 16,
						 "WM_DELETE_WINDOW");
    NUS_window_->delete_reply = xcb_intern_atom_reply(NUS_window_->connection,
						      NUS_window_->delete_cookie, 0);

    xcb_change_property(NUS_window_->connection, XCB_PROP_MODE_REPLACE,
			NUS_window_->window, (*NUS_window_->reply).atom,
			4, 32, 1, &(*NUS_window_->delete_reply).atom);
    
    xcb_map_window(NUS_window_->connection, NUS_window_->window);
    xcb_flush(NUS_window_->connection);
  }
#endif
  return NUS_SUCCESS;
}

void nus_window_free(NUS_window *NUS_window_)
{
  free(NUS_window_->reply);
  free(NUS_window_->delete_reply);
  
  xcb_disconnect(NUS_window_->connection);
  free(NUS_window_->title);
}

void nus_window_print(NUS_window NUS_window_)
{
  printf("NUS_window has title: %s\ndimensions:\nw: %d\nh: %d\n",
	 NUS_window_.title, NUS_window_.width, NUS_window_.height);
}
