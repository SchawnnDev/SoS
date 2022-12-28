#include "asm.h"

int asm_syscall(syscall_t type) {
    log_trace("asm_syscall of type %s", stringFromSyscall(type))
    asm_code_printf("li $v0, %d", type)
    asm_code_printf("syscall")
    return RETURN_SUCCESS;
}

int asm_writeAsciiz(const char* label, const char* content) {
    log_trace("asm_writeAsciiz: %s", content)
    asm_data_printf("\t%s: .asciiz %s", label, content)
    return RETURN_SUCCESS;
}