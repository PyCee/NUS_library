#include "NUS_key.h"

NUS_bool nus_key_unlock(NUS_key key, NUS_key lock)
{
  return ((key & lock) == lock);
}
