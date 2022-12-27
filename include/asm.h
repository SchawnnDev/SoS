#ifndef SOS_ASM_H
#define SOS_ASM_H

#include <errno.h>
#include "compilation.h"

#define asm_code_printf(...) \
    do { \
        char buf[ASM_PRINTF_BUF_MAX]; \
        if(snprintf(buf, ASM_PRINTF_BUF_MAX, __VA_ARGS__) < 0) { \
            log_error(strerror(errno));  \
            return RETURN_FAILURE; \
        } \
        addIntoCode(listInstruction, buf); \
    } while (0);

extern ListInstruction listInstruction;

#endif //SOS_ASM_H
