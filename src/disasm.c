#include <capstone/capstone.h>
#include <inttypes.h>
#include "disasm.h"
#include <stdio.h>
#include <string.h>



static void format_mnemonic(char *dst, size_t dst_size, const cs_insn *ins) {
    if (ins -> op_str[0] != '\0') {
        snprintf(dst, dst_size, "%s %s", ins -> mnemonic, ins -> op_str);
    } else {
        snprintf(dst, dst_size, "%s", ins -> mnemonic);
    }
}


static void write_plain(FILE *out, const cs_insn *ins) {
    char line[512];
    format_mnemonic(line, sizeof(line), ins);
    fprintf(out, "%s\n", line);
}


static void write_full(FILE *out, const cs_insn *ins) {
    char line[512];
    format_mnemonic(line, sizeof(line), ins);

    fprintf(out, "0x%016" PRIx64 ":  ", ins->address);

    for (int b = 0; b < ins -> size; b++) {
        fprintf(out, "%02x ", ins -> bytes[b]);
    }
    for (int b = ins -> size; b < 8; b++) {
        fprintf(out, "   ");
    }

    fprintf(out, " %s\n", line);
}


static void write_instruction(FILE *out, const cs_insn *ins, DisasmFormat fmt) {
    if (fmt == DISASM_PLAIN) {
        write_plain(out, ins);
    } else {
        write_full(out, ins);
    }
}


int write_disasm(const ByteBuf *buf, const char *out_path, DisasmFormat fmt) {
    if (buf -> len == 0) {
        fprintf(stderr, "Error: nothing to disassemble (0 bytes parsed)\n");

        return -1;
    }

    csh handle;
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
        fprintf(stderr, "Error: failed to initialize сapstone\n");

        return -1;
    }

    cs_insn *insn;
    size_t count = cs_disasm(handle, buf -> bytes, buf -> len, 0, 0, &insn);

    FILE *out = fopen(out_path, "w");

    if (!out) {
        perror("fopen");

        if (count > 0) {
            cs_free(insn, count);
        }

        cs_close(&handle);

        return -1;
    }

    size_t decoded_bytes = 0;
    
    for (size_t i = 0; i < count; i++) {
        write_instruction(out, &insn[i], fmt);
        decoded_bytes += insn[i].size;
    }

    if (decoded_bytes < buf -> len) {
        fprintf(out, "; stopped at byte offset %zu: undecodable sequence\n", decoded_bytes);
    }

    fclose(out);

    if (count > 0) {
        cs_free(insn, count);
    }
    cs_close(&handle);

    return 0;
}