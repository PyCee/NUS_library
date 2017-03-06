#include "NUS_reference_vector.h"
#include <stdio.h>

NUS_reference_vector nus_reference_vector_build_static(NUS_vector vector)
{
  NUS_reference_vector reference_vector;
  reference_vector.static_ref.type = NUS_REFERENCE_VECTOR_STATIC;
  reference_vector.static_ref.vector = vector;
  return reference_vector;
}
NUS_reference_vector nus_reference_vector_build_dynamic(NUS_vector *p_vector)
{
  NUS_reference_vector reference_vector;
  reference_vector.dynamic_ref.type = NUS_REFERENCE_VECTOR_DYNAMIC;
  reference_vector.dynamic_ref.p_vector = p_vector;
  return reference_vector;
}
NUS_reference_vector nus_reference_vector_build_difference
(NUS_vector *p_vector_0, NUS_vector *p_vector_1)
{
  NUS_reference_vector reference_vector;
  reference_vector.difference_ref.type = NUS_REFERENCE_VECTOR_DIFFERENCE;
  reference_vector.difference_ref.p_vector_0 = p_vector_0;
  reference_vector.difference_ref.p_vector_1 = p_vector_1;
  return reference_vector;
}

NUS_vector nus_reference_vector_get(NUS_reference_vector reference_vector)
{
  switch(reference_vector.type){
  case NUS_REFERENCE_VECTOR_STATIC:
    return reference_vector.static_ref.vector;
  case NUS_REFERENCE_VECTOR_DYNAMIC:
    return *reference_vector.dynamic_ref.p_vector;
  case NUS_REFERENCE_VECTOR_DIFFERENCE:
    return nus_vector_subtract(*reference_vector.difference_ref.p_vector_0,
			       *reference_vector.difference_ref.p_vector_1);
  default:
    printf("ERROR::Invalid reference vector type\n");
    return nus_vector_build(0.0, 0.0, 0.0);
  }
}

void nus_reference_vector_print(NUS_reference_vector reference_vector)
{
  switch(reference_vector.type){
  case NUS_REFERENCE_VECTOR_STATIC:
    printf("Static ");
    break;
  case NUS_REFERENCE_VECTOR_DYNAMIC:
    printf("Dynamic ");
    break;
  case NUS_REFERENCE_VECTOR_DIFFERENCE:
    printf("Difference ");
    break;
  default:
    printf("ERROR::Invalid reference vector type\n");
  }
  printf("reference vector: ");
  nus_vector_print(nus_reference_vector_get(reference_vector));
}
