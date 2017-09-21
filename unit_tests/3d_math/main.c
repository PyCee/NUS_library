#include <stdio.h>

#include <NUS/NUS_library.h>
#include <math.h>

#define PROGRAM_NAME "unit_test-3d_math"
#define UNIT_TEST_LOG_ERROR( ... )		\
  printf("UNIT_TEST_LOG::ERROR::" __VA_ARGS__)

int main(int argc, char *argv[])
{
  NUS_vector test_vector,
    test_vector_1;
  NUS_quaternion test_quaternion,
    test_quaternion_1;
  NUS_axes test_axes;
  NUS_matrix test_matrix;
  float rad;
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  // Test vector math
  // Test vector addition
  test_vector = nus_vector_add(nus_vector_build(3.0, 2.0, 1.0),
			nus_vector_build(1.0, 3.3, 0.7071));
  if(nus_vector_cmp(test_vector, nus_vector_build(4.0, 5.3, 1.7071), 0.00001)
     == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to add vectors\n");
    return -1;
  }
  // Test vector normalization
  test_vector = nus_vector_normalize(nus_vector_build(1.0, 0.0, 1.0));
  if(nus_vector_cmp(test_vector, nus_vector_build(0.7071, 0.0, 0.7071), 0.00001)
     == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to normalize vector\n");
    return -1;
  }

  // Test quaternion math
  // Test no rotation
  test_vector = nus_vector_build(0.0, 0.0, 1.0);
  test_quaternion = nus_quaternion_unit(nus_vector_build(0.0, 1.0, 0.0), 0);
  test_vector = nus_quaternion_apply_rotation(test_quaternion, test_vector);
  if(nus_vector_cmp(test_vector, nus_vector_build(0.0, 0.0, 1.0),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use quaternion rotation 1\n");
    return -1;
  }
  // Test quarter cirle rotation
  test_vector = nus_vector_build(0.0, 0.0, 1.0);
  test_quaternion = nus_quaternion_unit(nus_vector_build(0.707106, 0.707106, 0.0),
					3.14159/4.0);
  test_vector = nus_quaternion_apply_rotation(test_quaternion, test_vector);
  if(nus_vector_cmp(test_vector, nus_vector_build(0.5,-0.5, 0.7071),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use quaternion rotation 2\n");
    return -1;
  }

  test_quaternion = nus_quaternion_unit(nus_vector_build(0.0, 1.0, 0.0),
				        0);
  test_quaternion_1 = nus_quaternion_unit(nus_vector_build(0.0, 1.0, 0.0),
					3.14159/2.0);
  // Test lerp
  // Test lerp of 0
  test_vector = nus_vector_build(0.0, 0.0, 1.0);
  test_vector = nus_quaternion_apply_rotation(nus_quaternion_lerp(test_quaternion,
								  test_quaternion_1,
								  0.0), test_vector);
  if(nus_vector_cmp(test_vector, nus_vector_build(0.0, 0.0, 1.0),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use quaternion lerp 0\n");
    return -1;
  }

  // Test lerp of 0.5
  test_vector = nus_vector_build(0.0, 0.0, 1.0);
  test_vector = nus_quaternion_apply_rotation(nus_quaternion_lerp(test_quaternion,
								  test_quaternion_1,
								  0.5), test_vector);
  if(nus_vector_cmp(test_vector, nus_vector_build(0.707106, 0.0, 0.707106),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use quaternion lerp 0.5\n");
    return -1;
  }
  // Test lerp of 2
  test_vector = nus_vector_build(0.0, 0.0, 1.0);
  test_vector = nus_quaternion_apply_rotation(nus_quaternion_lerp(test_quaternion,
								  test_quaternion_1,
								  2.0), test_vector);
  if(nus_vector_cmp(test_vector, nus_vector_build(1.0, 0.0, 0.0),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use quaternion lerp 2\n");
    return -1;
  }
  // Test slerp
  // Test slerp of 0
  test_vector = nus_vector_build(0.0, 0.0, 1.0);
  test_vector = nus_quaternion_apply_rotation(nus_quaternion_slerp(test_quaternion,
								  test_quaternion_1,
								  0.0), test_vector);
  if(nus_vector_cmp(test_vector, nus_vector_build(0.0, 0.0, 1.0),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use quaternion slerp 0\n");
    return -1;
  }
  // Test slerp of 0.5
  test_vector = nus_vector_build(0.0, 0.0, 1.0);
  test_vector = nus_quaternion_apply_rotation(nus_quaternion_slerp(test_quaternion,
								  test_quaternion_1,
								  0.5), test_vector);
  if(nus_vector_cmp(test_vector, nus_vector_build(0.707106, 0.0, 0.707106),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use quaternion slerp 0.5\n");
    return -1;
  }
  // Test slerp of 2
  test_vector = nus_vector_build(0.0, 0.0, 1.0);
  test_vector = nus_quaternion_apply_rotation(nus_quaternion_slerp(test_quaternion,
								  test_quaternion_1,
								  2.0), test_vector);
  if(nus_vector_cmp(test_vector, nus_vector_build(1.0, 0.0, 0.0),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use quaternion slerp 2\n");
    return -1;
  }
  
  // Test axes math
  // Rotate axes by pi/2
  test_axes = nus_axes_global_yaw(nus_axes_build(nus_vector_build(-1.0, 0.0, 0.0),
						 nus_vector_build(0.0, 1.0, 0.0),
						 nus_vector_build(0.0, 0.0, 1.0)),
				  3.14159265359/2.0);
  if(nus_axes_cmp(test_axes, nus_axes_build(nus_vector_build(0.0, 0.0, 1.0),
					    nus_vector_build(0.0, 1.0, 0.0),
					    nus_vector_build(1.0, 0.0, 0.0)),
		  0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to rotate axes\n");
    return -1;
  }

  // Test matrix math
  // Test translation matrix
  test_matrix = nus_matrix_build_translation(nus_vector_build(7.3, -2.1, 0.77));
  test_vector = nus_matrix_transform(test_matrix, nus_vector_build(-2.3, 1.0, 7.0));
  if(nus_vector_cmp(nus_vector_build(5.0, -1.1, 7.77),
		    test_vector, 0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to translate vector\n");
    return -1;
  }

  // Test rotation matrix 1
  test_quaternion = nus_quaternion_unit(nus_vector_build(0.0, 1.0, 0.0), 0.0);
  test_axes = nus_axes_build(nus_vector_build(0.0, 0.0, 1.0),
			     nus_vector_build(0.0, 1.0, 0.0),
			     nus_vector_build(1.0, 0.0, 0.0));
  test_axes = nus_axes_global_rotation(test_axes, test_quaternion);
  test_matrix = nus_matrix_build_rotation(test_axes);
  test_vector = nus_matrix_transform(test_matrix, nus_vector_build(0.0, 0.0, 1.0));
  if(nus_vector_cmp(nus_vector_build(0.0, 0.0, 1.0),
		    test_vector, 0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use rotation matrix\n");
    return -1;
  }
  
  // Test rotation matrix 2
  test_quaternion = nus_quaternion_unit(nus_vector_build(0.0, 1.0, 0.0), 3.14159/2.0);
  test_axes = nus_axes_build(nus_vector_build(0.0, 0.0, 1.0),
			     nus_vector_build(0.0, 1.0, 0.0),
			     nus_vector_build(1.0, 0.0, 0.0));
  test_axes = nus_axes_global_rotation(test_axes, test_quaternion);
  test_matrix = nus_matrix_build_rotation(test_axes);
  test_vector = nus_matrix_transform(test_matrix, nus_vector_build(0.0, 0.0, 1.0));
  if(nus_vector_cmp(nus_vector_build(-1.0, 0.0, 0.0),
		    test_vector, 0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use rotation matrix\n");
    return -1;
  }

  
  // Test quaternion to rotation matrix
  test_vector = nus_vector_build(0.856916, 0.515366, 0.009584);
  test_vector_1 = nus_vector_build(0.0, 1.0, 0.0);
  rad = acos(nus_vector_dot(test_vector, test_vector_1));
  test_vector_1 = nus_vector_normalize(nus_vector_cross(test_vector, test_vector_1));
  test_quaternion = nus_quaternion_unit(test_vector_1, rad);
  test_matrix = nus_matrix_build_rotation_qua(test_quaternion);
  test_vector = nus_matrix_transform(test_matrix, test_vector);
  
  if(nus_vector_cmp(test_vector, nus_vector_build(0.0, 1.0, 0.0),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to create rotation matrix from quaternion\n");
    return -1;
  }
  
  // Test transformation matrix (ordering translation and rotation)
  test_quaternion = nus_quaternion_unit(nus_vector_build(0.0, 1.0, 0.0), 3.14159/1.5);
  test_axes = nus_axes_build(nus_vector_build(0.0, 0.0, 1.0),
			     nus_vector_build(0.0, 1.0, 0.0),
			     nus_vector_build(1.0, 0.0, 0.0));
  test_axes = nus_axes_global_rotation(test_axes, test_quaternion);
  test_vector = nus_vector_build(-1.5, 3.14159, 2.1);
  test_matrix = nus_matrix_build_transformation(nus_vector_build(1.2, 2.3, 3.4),
						nus_vector_build(1.0, 1.0, 1.0),
						test_axes);
  test_vector = nus_matrix_transform(test_matrix, test_vector);
  if(nus_vector_cmp(nus_vector_build(0.131343, 5.441590, 1.050964),
		    test_vector, 0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to use rotation matrix\n");
    return -1;
  }

  // Test inverse matrix
  // Inverts and applies the transformation from the test
  //   "Test transformation matrix (ordering translation and rotation)" to get
  //   the original vector
  test_matrix = nus_matrix_inverse(test_matrix);
  test_vector = nus_matrix_transform(test_matrix, test_vector);
  if(nus_vector_cmp(test_vector, nus_vector_build(-1.5, 3.14159, 2.1),
		    0.00001) == NUS_FALSE){
    UNIT_TEST_LOG_ERROR("failed to invert matrix\n");
    return -1;
  }
  
  printf("Math tested successfully\n");
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
