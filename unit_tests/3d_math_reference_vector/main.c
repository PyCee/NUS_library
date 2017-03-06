#include <stdio.h>

#include <NUS/NUS_library.h>

#define PROGRAM_NAME "unit_test-3d_math_reference_vector"

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  NUS_reference_vector reference_vector;
  NUS_vector vector_0;
  NUS_vector vector_1;

  printf("Building static reference vector to {0.0, 1.0, 2.0}\n");
  reference_vector =
    nus_reference_vector_build_static(nus_vector_build(0.0, 1.0, 2.0));
  nus_reference_vector_print(reference_vector);

  printf("\nBuilding dynamic reference vector to {0.0, 1.0, 2.0}\n");
  vector_0 = nus_vector_build(0.0, 1.0, 2.0);
  reference_vector = nus_reference_vector_build_dynamic(&vector_0);
  nus_reference_vector_print(reference_vector);
  printf("Changing vector to {3.0, 4.0, 5.0}\nReference Vector: ");
  vector_0 = nus_vector_build(3.0, 4.0, 5.0);
  nus_reference_vector_print(reference_vector);

  printf("\nBuilding difference reference vector between {0.0, 0.0, 1.0} and {0.0, 0.0, 3.0}\n");
  vector_0 = nus_vector_build(0.0, 0.0, 1.0);
  vector_1 = nus_vector_build(0.0, 0.0, 3.0);
  reference_vector = nus_reference_vector_build_difference(&vector_0, &vector_1);
  nus_reference_vector_print(reference_vector);
  printf("Changing difference to be between {0.0, 0.0, 0.0} and {0.0, 0.0, 4.0}\nReference Vector: ");
  vector_0 = nus_vector_build(0.0, 0.0, 0.0);
  vector_1 = nus_vector_build(0.0, 0.0, 4.0);
  nus_reference_vector_print(reference_vector);
  
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
