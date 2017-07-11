#include "NUS_uniform_buffer.h"
#include "../NUS_log.h"

void (*nus_uniform_buffer_free)(NUS_uniform_buffer *) = nus_memory_map_free;
NUS_result (*nus_uniform_buffer_flush)(NUS_uniform_buffer, void *) =
  nus_memory_map_flush;

NUS_result nus_uniform_buffer_build
(size_t size, NUS_uniform_buffer *p_uniform_buffer)
{
  if(nus_memory_map_build(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, p_uniform_buffer)
     != NUS_SUCCESS){
    NUS_LOG_ERROR("failed ot build uniform buffer with size %zu\n", size);
    return NUS_FAILURE;
  }
  return NUS_SUCCESS;
}
