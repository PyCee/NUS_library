#include <stdio.h>

#include <NUS/NUS_engine.h>

#define PROGRAM_NAME "unit_test-3d_math"

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  
  printf("\n\nStarting Vector Tests:\n\nShould print {0.0, 0.0, 1.0}\n");
  nus_vector_print(nus_vector_build(0.0, 0.0, 1.0));
  printf("{3.0, 2.0, 1.0} + {1.0, 3.3, 0.7071}\n");
  nus_vector_print(nus_vector_add(nus_vector_build(3.0, 2.0, 1.0),
				  nus_vector_build(1.0, 3.3, 0.7071)));
  printf("Normalized {1.0, 0.0, 1.0}\n");
  nus_vector_print(nus_vector_normalize(nus_vector_build(1.0, 0.0, 1.0)));

  
  printf("\n\nStarting Matrix Tests:\n\nIdentity Matrix:\n");
  nus_matrix_print(nus_matrix_identity());
  printf("Translate matrix by {7.3, -2.1, 0.77}:\n");
  nus_matrix_print(nus_matrix_translation(nus_vector_build(7.3, -2.1, 0.77)));
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
