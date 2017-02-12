#include "NUS_executable_path.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../NUS_os.h"

#if defined(NUS_OS_WINDOWS)

#elif defined(NUS_OS_UNIX)
#include <unistd.h>
#include <libgen.h>
#endif

char *nus_executable_path_prefix(char *relative_path)
{
  char buffer[100] = {0},
    *dir_path,
      *absolute_path;
  
  // Obtain absolute path up to but not including the executable for each os
#if defined(NUS_OS_WINDOWS)
    
#elif defined(NUS_OS_UNIX)
    if(readlink("/proc/self/exe", buffer, 100) == -1){
      printf("ERROR::failed to obtain executable's absolute path\n");
    return NULL;
  }
  dir_path = dirname(buffer);
#endif
  absolute_path = malloc(strlen(dir_path) + strlen(relative_path) + 2);
  strcpy(absolute_path, dir_path);
  strcat(absolute_path, "/");
  strcat(absolute_path, relative_path);
  return absolute_path;
}
