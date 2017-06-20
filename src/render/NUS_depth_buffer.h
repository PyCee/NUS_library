#ifndef NUS_DEPTH_BUFFER_H
#define NUS_DEPTH_BUFFER_H

#include "../NUS_result.h"
#include "NUS_texture.h"

typedef NUS_texture NUS_depth_buffer;

NUS_result nus_depth_buffer_build
(unsigned int, unsigned int, NUS_depth_buffer *);
void (*nus_depth_buffer_free)(NUS_depth_buffer *) = nus_texture_free;

#endif /* NUS_DEPTH_BUFFER_H */
