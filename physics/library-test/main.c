#include <center.h>
#include <stdio.h>
#include <stdlib.h>

center build_c(void)
{
    center cen = gen_center();
    set_directional_gravity(&cen, build_vector(0.0, -1.0, 0.0));
    return cen;
}

int main(int argc, char *args[])
{
  
  center asd = build_c();
  batch_update_center(&asd, 0, 0, 1, 0.640);
  print_center(asd);
  printf("Freeing physics test\n");
  free_center(&asd);
  return 0;
}
