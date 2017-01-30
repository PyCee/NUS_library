#ifndef _NUS_WINDOW_H_
#define _NUS_WINDOW_H_

#include "../NUS_result.h"
#include "../NUS_os.h"

#if defined(NUS_OS_WINDOWS)

#elif defined(NUS_OS_UNIX)
#include <xcb/xcb.h>
#endif

typedef struct NUS_window{
  char *title;
  unsigned short width, height;
#if defined(NUS_OS_WINDOWS)

#elif defined(NUS_OS_UNIX)
  xcb_connection_t *connection;
  xcb_screen_t *screen;
  xcb_window_t window;

  xcb_intern_atom_cookie_t cookie;
  xcb_intern_atom_reply_t* reply;
  xcb_intern_atom_cookie_t delete_cookie;
  xcb_intern_atom_reply_t* delete_reply;
#endif
} NUS_window;
NUS_result nus_window_build(char *, unsigned short, unsigned short, NUS_window *);
void nus_window_free(NUS_window *);
void nus_window_print(NUS_window);

#endif /* _NUS_WINDOW_H_ */

