#ifndef NUS_HANDLE_H
#define NUS_HANDLE_H

#define NUS_NULL_HANDLE NULL

#define NUS_DEFINE_HANDLE( handle_name )	\
  typedef struct handle_name##_t *handle_name

// Recieve underlying data from Handle
#define NUS_REC_HANDLE( handle )		\
  (*handle)

#endif /* NUS_HANDLE_H */
