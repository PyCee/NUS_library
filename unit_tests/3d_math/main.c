#include <stdio.h>

#include <NUS/NUS_library.h>

#define PROGRAM_NAME "unit_test-3d_math"

char nus_axes_cmp(NUS_axes, NUS_axes, float);//TODO: impliment in library
char nus_matrix_cmp(NUS_matrix, NUS_matrix, float);//TODO: impliment in library


int main(int argc, char *argv[])
{
  NUS_vector test_vector;
  NUS_axes test_axes;
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}
  
  test_vector = nus_vector_build(0.0, 0.0, 1.0);
  if(test_vector.x != 0.0 || test_vector.y != 0.0 || test_vector.z != 1.0){
    printf("UNIT_TEST_ERROR::failed to build vector\n");
    return -1;
  }
  
  test_vector = nus_vector_add(nus_vector_build(3.0, 2.0, 1.0),
			nus_vector_build(1.0, 3.3, 0.7071));
  if(nus_vector_cmp(test_vector, nus_vector_build(4.0, 5.3, 1.7071), 0.00001) == 0){
    printf("UNIT_TEST_ERROR::failed to add vectors\n");
    return -1;
  }
  
  test_vector = nus_vector_normalize(nus_vector_build(1.0, 0.0, 1.0));
  if(nus_vector_cmp(test_vector, nus_vector_build(0.7071, 0.0, 0.7071), 0.00001) == 0){
    printf("UNIT_TEST_ERROR::failed to normalize vector\n");
    return -1;
  }
  
  //TODO: axes
  test_axes = nus_axes_build(nus_vector_build(0.0, 0.0, -1.0),
			     nus_vector_build(0.0, 1.0, 0.0),
			     nus_vector_build(-1.0, 0.0, 0.0));
  if((nus_vector_cmp(test_axes.forward, nus_vector_build(0.0, 0.0, -1.0), 0.00001) &&
      nus_vector_cmp(test_axes.upward, nus_vector_build(0.0, 1.0, 0.0), 0.00001) &&
      nus_vector_cmp(test_axes.left, nus_vector_build(-1.0, 0.0, 0.0), 0.00001)) == 0){
    printf("UNIT_TEST_ERROR::failed to build axes\n");
    return -1;
  }

  //todo
  printf("\nBasic Axes yaw by pi/2:\n");
  test_axes = nus_axes_global_yaw(test_axes, 3.14159265359/2.0);
  nus_axes_print(test_axes);


  //TODO: matrices

  printf("Math tested successfully\n");
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}

char nus_axes_cmp(NUS_axes axes_0, NUS_axes axes_1, float d)//TODO: impliment in library
{
  return nus_vector_cmp(axes_0.forward, axes_1.forward, d) &&
    nus_vector_cmp(axes_0.upward, axes_1.upward, d) &&
    nus_vector_cmp(axes_0.left, axes_1.left, d);
}
//char nus_matrix_cmp(NUS_matrix, NUS_matrix, float);//TODO: impliment in library
