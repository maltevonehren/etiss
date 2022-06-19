// License: https://github.com/tum-ei-eda/etiss/blob/master/LICENSE

#include <stdio.h>
#include <stdlib.h>

#define ERROR_HALT(msg)                \
    printf("%s at %08X\n", msg, mepc); \
    exit(-1);

void _trap_handler_c(unsigned int mcause, unsigned int mepc)
{
    switch (mcause) {
    case 0x0:
        ERROR_HALT("Instruction address misaligned");
    case 0x1:
        ERROR_HALT("Instruction access fault");
    case 0x2:
        ERROR_HALT("Illegal instruction");
    case 0x4:
        ERROR_HALT("Load address misaligned");
    case 0x5:
        ERROR_HALT("Load access fault");
    case 0x6:
        ERROR_HALT("Store/AMO address misaligned");
    case 0x7:
        ERROR_HALT("Store/AMO access fault");
    case 0xb: // Machine ECALL
        ERROR_HALT("ECALL");
    default:
        ERROR_HALT("Unhandled cause");
    }
}
