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


  printf("\n\nStarting Axes Tests:\n\nBasic Axes:\n");
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

  
  printf("\n\nStarting Matrix Tests:\n\nIdentity Matrix:\n");
  nus_matrix_print(nus_matrix_identity());
  printf("Translate matrix by {7.3, -2.1, 0.77}:\n");
  nus_matrix_print(nus_matrix_translation(nus_vector_build(7.3, -2.1, 0.77)));

  NUS_axes t_rotation = nus_axes_build(nus_vector_build(0.0, 0.0, -1.0),
				       nus_vector_build(0.0, 1.0, 0.0),
				       nus_vector_build(-1.0, 0.0, 0.0));
  NUS_vector t_translation = nus_vector_build(-3.0, 4.0, -5.0);
  NUS_matrix transformation = nus_matrix_transformation(t_translation, t_rotation);
  printf("Scale: %f\nRotation:\n", 1.0);
  nus_axes_print(t_rotation);
  printf("Translation:\n");
  nus_vector_print(t_translation);
  printf("Transformation matrix:\n");
  nus_matrix_print(transformation);
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
