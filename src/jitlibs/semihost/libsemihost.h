#ifndef JITLIB_SEMIHOST_H
#define JITLIB_SEMIHOST_H

#include <stdint.h>

struct ETISS_CPU;
struct ETISS_System;

int64_t etiss_semihost(ETISS_CPU *const cpu, ETISS_System *const etissSystem, void *const *const _, uint32_t XLEN,
                       uint64_t operation, uint64_t parameter);

#endif
