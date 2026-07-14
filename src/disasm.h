#ifndef DISASM_H
#define DISASM_H

#include "parser.h"

typedef enum {
    DISASM_PLAIN,
    DISASM_FULL
} DisasmFormat;

int write_disasm(const ByteBuf *buf, const char *out_path, DisasmFormat fmt);

#endif