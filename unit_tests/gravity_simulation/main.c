
#include <stdio.h>
#include <string.h>

#include <NUS/NUS_library.h>
#include <limits.h>

#define PROGRAM_NAME "unit_test-gravity_simulation"

char run;
void close_win(void);

int main(int argc, char *argv[])
{
  printf("starting unit test %s\n", PROGRAM_NAME);
  if(argc){}
  if(argv){}

  NUS_mass test_mass = nus_mass_build();
  nus_mass_print(test_mass);

  nus_movement_set_mass(test_mass.movement, 1);
  nus_movement_set_gravity(test_mass.movement, nus_vector_build(0.0, -1.0, 0.0));

  nus_mass_batch_update(&test_mass, 0, 0, 1, 2.0);
  
  nus_mass_print(test_mass);
  nus_mass_free(&test_mass);
  
  printf("unit test %s completed\n", PROGRAM_NAME);
  return 0;
}
