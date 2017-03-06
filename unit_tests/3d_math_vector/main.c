#include <stdio.h>

#include <NUS/NUS_library.h>

#define PROGRAM_NAME "unit_test-3d_math_vector"

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}
  
  printf("Should print {0.0, 0.0, 1.0}\n");
  nus_vector_print(nus_vector_build(0.0, 0.0, 1.0));
  printf("{3.0, 2.0, 1.0} + {1.0, 3.3, 0.7071}\n");
  nus_vector_print(nus_vector_add(nus_vector_build(3.0, 2.0, 1.0),
				  nus_vector_build(1.0, 3.3, 0.7071)));
  printf("Normalized {1.0, 0.0, 1.0}\n");
  nus_vector_print(nus_vector_normalize(nus_vector_build(1.0, 0.0, 1.0)));
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
