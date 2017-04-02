#ifndef NUS_SAVE_H
#define NUS_SAVE_H

#include "NUS_result.h"
#include "strings/NUS_string_group.h"
#include "strings/NUS_absolute_path.h"

#define NUS_SAVE_FILE_HEAD "NUS_save_file"
#define NUS_SAVE_FILE_MAX_LINE_LENGTH 100

typedef struct NUS_save{
  // Each even string in "contents" is an element
  // Every odd string is the value of the element before it
  NUS_string_group contents;
} NUS_save;

NUS_result nus_save_build(NUS_absolute_path, NUS_save *);
NUS_result nus_save_output(NUS_absolute_path, NUS_save);
NUS_result nus_save_set_variable(NUS_save *, char const * const, char const * const);
void nus_save_free(NUS_save *);
NUS_result nus_save_get_value(NUS_save, char const * const, char **);

#endif /* NUS_SAVE_H */
