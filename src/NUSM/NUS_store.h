#ifndef NUSM_STORE_H
#define NUSM_STORE_H

#include "../NUS_result.h"
#include "../model/NUS_binary_model.h"

#define NUSM_MAGIC_STRING "NUSM"

NUS_result nusm_store(char *, struct NUS_binary_model *);

#endif /* NUSM_STORE_H */
