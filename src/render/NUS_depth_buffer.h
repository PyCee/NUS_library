#ifndef NUS_DEPTH_BUFFER_H
#define NUS_DEPTH_BUFFER_H

#include "../NUS_result.h"
#include "NUS_texture.h"

struct NUS_gpu;

typedef NUS_texture NUS_depth_buffer;

NUS_result nus_depth_buffer_build
(struct NUS_gpu, unsigned int, unsigned int, NUS_depth_buffer *);

#endif /* NUS_DEPTH_BUFFER_H */
