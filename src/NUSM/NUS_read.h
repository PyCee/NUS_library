#ifndef NUSM_READ_H
#define NUSM_READ_H

#include "../NUS_result.h"
#include <stdint.h>
#include "../model/NUS_binary_model.h"

NUS_result nusm_read(char *, struct NUS_binary_model *);

#endif /* NUSM_READ_H */
