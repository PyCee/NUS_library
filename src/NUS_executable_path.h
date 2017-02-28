#ifndef NUS_EXECUTABLE_PATH_H
#define NUS_EXECUTABLE_PATH_H

/* 
This is used to allow the executable to be run when it relies on
resources relative to the executable's path
Such as:
  shader binaries
  save files
*/

char * nus_executable_path_prefix(char const * const);
// In-Application:
// returns a malloced string
// free returned string after use

#endif /* NUS_EXECUTABLE_PATH_H */
