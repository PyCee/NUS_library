#include <stdio.h>

#include <NUS/NUS_library.h>

#define PROGRAM_NAME "unit_test-3d_math_axes"

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  printf("Basic Axes:\n");
  NUS_axes basic_axes = nus_axes_build(nus_vector_build(0.0, 0.0, -1.0),
				       nus_vector_build(0.0, 1.0, 0.0),
				       nus_vector_build(-1.0, 0.0, 0.0));
  nus_axes_print(basic_axes);
  
  printf("\nBasic Axes yaw by pi/2:\n");
  basic_axes = nus_axes_global_yaw(basic_axes, 3.14159265359/2.0);
  nus_axes_print(basic_axes);
  
  printf("\nYaw by another pi:\n");
  basic_axes = nus_axes_global_yaw(basic_axes, 3.14159265359);
  nus_axes_print(basic_axes);
  
  printf("\nBack to the basic axes with another pi/2:\n");
  basic_axes = nus_axes_global_yaw(basic_axes, 3.14159265359/2.0);
  nus_axes_print(basic_axes);

  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
