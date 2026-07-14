#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdint.h>
#include "parser.h"

typedef struct {
    uint64_t rax, rbx, rcx, rdx;
    uint64_t rsi, rdi, rbp, rsp;
    uint64_t r8, r9, r10, r12, r13, r14, r15;
} RegState;

int execute_bytes(const ByteBuf *buf, RegState *regs_out);

#endif