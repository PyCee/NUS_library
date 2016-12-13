#ifndef _KINEMATIC_PROPERTY_H_
#define _KINEMATIC_PROPERTY_H_

#include <vector.h>

#define DEFAULT_KINEMATIC_FACTOR {DEFAULT_VECTOR, 0.0, 0}
typedef struct kinematic_factor{
    vector value;
    float scale;
    unsigned short id;
} kinematic_factor;

kinematic_factor gen_kinematic_factor(void);
void free_kinematic_factor(kinematic_factor *);
kinematic_factor build_kinematic_factor(vector , float, short);
void print_kinematic_factor(kinematic_factor);

#define DEFAULT_KINEMATIC_PROPERTY {NULL, DEFAULT_VECTOR, 0}
typedef struct kinematic_property{
  kinematic_factor *factors;
  vector value;
  short num_factors;
} kinematic_property;

kinematic_property gen_kinematic_property(void);
void free_kinematic_property(kinematic_property *);
vector update_kinematic_property(kinematic_property *, double);
void replace_kinematic_factor(kinematic_property *, vector, float, short);
short add_kinematic_factor(kinematic_property *, vector, float);
void remove_kinematic_factor(kinematic_property *, short);
void add_instant_kinematic_factor(kinematic_property *, vector);
void print_kinematic_property(kinematic_property);

#endif /* _KINEMATIC_PROPERTY_H_ */
