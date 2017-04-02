
#include <stdio.h>
#include <string.h>

#include <NUS/NUS_library.h>
#include <limits.h>

#define PROGRAM_NAME "unit_test-save_file"

char run;
void close_win(void);

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}
  
  NUS_save save;
  if(nus_save_build(nus_absolute_path_build("save_file"), &save) != NUS_SUCCESS){
    printf("ERROR::failed to build save\n");
    return -1;
  }

  char *element = "game",
    *value;
  if(nus_save_get_value(save, element, &value) != NUS_SUCCESS){
    printf("ERROR::failed to get value from element \"%s\"\n", element);
    return -1;
  }
  printf("game is \"%s\"\n", value);

  
  element = "counter";
  if(nus_save_get_value(save, element, &value) != NUS_SUCCESS){
    printf("ERROR::failed to get value from element \"%s\"\n", element);
    return -1;
  }
  
  int counter = (int)strtol(value, NULL, 10);
  printf("previous counter = %d\nnew counter = %d\n", counter, counter + 1);
  ++counter;
  char counter_str[NUS_SAVE_FILE_MAX_LINE_LENGTH];
  sprintf(counter_str, "%d", counter);
  nus_save_set_variable(&save, "counter", counter_str);
  
  
  element = "whatis";
  if(nus_save_get_value(save, element, &value) != NUS_SUCCESS){
    printf("ERROR::failed to get value from element \"%s\"\n", element);
    return -1;
  }
  printf("whatis \"%s\"\n", value);

  nus_save_output(nus_absolute_path_build("save_file"), save);
  nus_save_free(&save);
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
