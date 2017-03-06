#ifndef NUS_REFERENCE_VECTOR_H
#define NUS_REFERENCE_VECTOR_H

#include "NUS_vector.h"

typedef enum{
  NUS_REFERENCE_VECTOR_STATIC,
  NUS_REFERENCE_VECTOR_DYNAMIC,
  NUS_REFERENCE_VECTOR_DIFFERENCE
} NUS_reference_vector_type;

struct NUS_reference_vector_static{
  NUS_reference_vector_type type;
  NUS_vector vector;
};
struct NUS_reference_vector_dynamic{
  NUS_reference_vector_type type;
  NUS_vector *p_vector;
};
struct NUS_reference_vector_difference{
  NUS_reference_vector_type type;
  NUS_vector *p_vector_0, *p_vector_1;
};
/*
struct NUS_reference_vector_interpolated{
  NUS_reference_vectoor_type type;
  double t;
  NUS_vector *p_vector_0, *p_vector_1;
}
*/

typedef union NUS_reference_vector{
  NUS_reference_vector_type type;
  struct NUS_reference_vector_static static_ref;
  struct NUS_reference_vector_dynamic dynamic_ref;
  struct NUS_reference_vector_difference difference_ref;
} NUS_reference_vector;

NUS_reference_vector nus_reference_vector_build_static(NUS_vector);
NUS_reference_vector nus_reference_vector_build_dynamic(NUS_vector *);
NUS_reference_vector nus_reference_vector_build_difference(NUS_vector *, NUS_vector *);

NUS_vector nus_reference_vector_get(NUS_reference_vector);
void nus_reference_vector_print(NUS_reference_vector);

#endif /* NUS_REFERENCE_VECTOR_H */
