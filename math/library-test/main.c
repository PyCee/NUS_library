#include "vector.h"
#include "matrix.h"
#include "quaternion.h"
#include "angle.h"
#include "pythag.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("Vectors\n");
	printf("Printing 3D-Vector [0.0, 0.0, 1.0]: ");
	print_vector(build_vector(0.0, 0.0, 1.0));
	printf("\nPrinting Sum Of [0.0, 0.0, 1.0] + [0.0, 1.0, 0.5] = ");
	print_vector(add_vector(build_vector(0.0, 0.0, 1.0), build_vector(0.0, 1.0, 0.5)));
	printf("\nPrinting Difference Of [0.0, 0.0, 1.0] - [0.0, 1.0, 0.5] = ");
	print_vector(sub_vector(build_vector(0.0, 0.0, 1.0), build_vector(0.0, 1.0, 0.5)));
	printf("\nPrinting Dot Product Of [0.0, 0.0, 1.0] . [1.0, 0.0, 0.0] = ");
	printf("%f\n", dot_vector(build_vector(0.0, 0.0, 1.0), build_vector(1.0, 0.0, 0.0)));
	printf("\nPrinting Cross Product Of [0.0, 0.0, 1.0] x [1.0, 0.0, 0.0] = ");
	print_vector(cross_vector(build_vector(0.0, 0.0, 1.0), build_vector(1.0, 0.0, 0.0)));
	
	printf("\nMatrices\n");
	printf("Printing Identitiy 4x4 Matrix\n");
	print_matrix(build_identity_matrix());
	
	printf("\nQuaternions\n");
	printf("Printing Pure Quaternion [0.0, 0.0, 0.0, 1.0]\n");
	print_quaternion(build_pure_quaternion(build_vector(0.0, 0.0, 1.0)));
	printf("\nPrinting Unit Quaternion\n");
	print_quaternion(build_unit_quaternion(build_vector(0.0, 0.0, 1.0), 90.0));
	printf("\nRotating Vector\n");
	print_vector(apply_rotation_quaternion(build_vector(0.7071, 0.701, 0.0), build_unit_quaternion(build_vector(0.0, 0.0, 1.0), 90.0)));
}
