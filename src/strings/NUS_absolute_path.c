#include "NUS_absolute_path.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../NUS_os.h"

#if defined(NUS_OS_WINDOWS)

#elif defined(NUS_OS_UNIX)
#include <unistd.h>
#include <libgen.h>
#endif

NUS_absolute_path nus_absolute_path_build(char const * const relative_path)
{
  NUS_absolute_path absolute_path;
  static char executable_path[NUS_ABSOLUTE_PATH_MAX_STRING_COUNT] = {0};
  
  if(executable_path[0] == 0){
    // If executable path has not yet been calculated
    // Obtain absolute path up to but not including the executable for each os
  
#if defined(NUS_OS_WINDOWS)
    
#elif defined(NUS_OS_UNIX)
    if(readlink("/proc/self/exe", executable_path, NUS_ABSOLUTE_PATH_MAX_STRING_COUNT) == -1){
      printf("ERROR::failed to obtain executable's absolute path\n");
      return absolute_path;
    }
    dirname(executable_path);
#endif
  }
  strcpy(absolute_path.path, executable_path);
  strcat(absolute_path.path, "/");
  strcat(absolute_path.path, relative_path);
  return absolute_path;
}
