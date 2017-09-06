#ifndef NUS_UNIFORM_BUFFER_H
#define NUS_UNIFORM_BUFFER_H

#include "NUS_memory_map.h"

typedef NUS_memory_map NUS_uniform_buffer;

NUS_result nus_uniform_buffer_build
(size_t, NUS_uniform_buffer *);
extern void (*nus_uniform_buffer_free)(NUS_uniform_buffer *);
extern NUS_result (*nus_uniform_buffer_flush)(NUS_uniform_buffer, void *, size_t);

#endif /* NUS_UNIFORM_BUFFER_H */
