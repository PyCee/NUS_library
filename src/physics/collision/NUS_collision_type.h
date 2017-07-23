#ifndef NUS_COLLISION_TYPE
#define NUS_COLLISION_TYPE

typedef enum NUS_collision_type{
  NUS_COLLISION_POINT,
  NUS_COLLISION_SPHERE,
  NUS_COLLISION_BOX,
  NUS_COLLISION_CAPSULE,
  NUS_COLLISION_SOUP//TODO: collider for poly-soup
} NUS_collision_type;

#endif /* NUS_COLLISION_TYPE */
