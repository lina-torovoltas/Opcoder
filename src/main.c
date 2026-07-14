#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "execution.h"
#include "disasm.h"




static void print_usage(const char *prog) {
    printf(
        "Usage: %s <input.opc> [options]\n"
        "\n"
        "Options:\n"
        "  -e          execute code in memory\n"
        "  -d          show registers after execution\n"
        "  -o [FILE]   write binary (default: output.bin)\n"
        "  -a [FILE]   disassemble (default: output.dasm)\n"
        "  -ao [FILE]  disassemble plain format (default: output.dasm)\n",
        prog
    );
}


static int is_option(const char *s) {
    return s[0] == '-';
}


static void print_regs(const RegState *r) {
    printf("rax = 0x%016llx\n", (unsigned long long)r->rax);
    printf("rbx = 0x%016llx\n", (unsigned long long)r->rbx);
    printf("rcx = 0x%016llx\n", (unsigned long long)r->rcx);
    printf("rdx = 0x%016llx\n", (unsigned long long)r->rdx);

    printf("rsi = 0x%016llx\n", (unsigned long long)r->rsi);
    printf("rdi = 0x%016llx\n", (unsigned long long)r->rdi);

    printf("r8  = 0x%016llx\n", (unsigned long long)r->r8);
    printf("r9  = 0x%016llx\n", (unsigned long long)r->r9);
    printf("r10 = 0x%016llx\n", (unsigned long long)r->r10);

    printf("r12 = 0x%016llx\n", (unsigned long long)r->r12);
    printf("r13 = 0x%016llx\n", (unsigned long long)r->r13);
    printf("r14 = 0x%016llx\n", (unsigned long long)r->r14);
    printf("r15 = 0x%016llx\n", (unsigned long long)r->r15);

    printf("rbp = 0x%016llx\n", (unsigned long long)r->rbp);
    printf("rsp = 0x%016llx\n", (unsigned long long)r->rsp);
}


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Error: no input file given\n");
        print_usage(argv[0]);

        return 1;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);

        return 0;
    }

    const char *in_path = argv[1];
    const char *out_path = NULL;
    const char *asm_path = NULL;

    int execute = 0;
    int output = 0;
    int debug = 0;
    int disasm = 0;

    DisasmFormat asm_fmt = DISASM_FULL;

    for (int i = 2; i < argc; i++) {

        if (strcmp(argv[i], "-e") == 0) {
            execute = 1;

        } else if (strcmp(argv[i], "-d") == 0) {
            debug = 1;

        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc && !is_option(argv[i + 1]))
                out_path = argv[++i];
            else
                out_path = "output.bin";

            output = 1;

        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "-ao") == 0) {
            int plain = (strcmp(argv[i], "-ao") == 0);

            if (i + 1 < argc && !is_option(argv[i + 1]))
                asm_path = argv[++i];
            else
                asm_path = "output.dasm";

            disasm = 1;
            asm_fmt = plain ? DISASM_PLAIN : DISASM_FULL;

        } else {
            fprintf(stderr, "Error: unknown argument '%s'\n", argv[i]);
            
            return 1;
        }
    }

    if (!output || debug)
        execute = 1;

    ByteBuf buf;

    if (parse_opc_file(in_path, &buf) != 0)

        return 1;

    printf("%zu bytes parsed\n", buf.len);

    if (output) {
        FILE *out = fopen(out_path, "wb");

        if (!out) {
            perror("fopen");
            bytebuf_free(&buf);

            return 1;
        }

        if (fwrite(buf.bytes, 1, buf.len, out) != buf.len) {
            perror("fwrite");
            fclose(out);
            bytebuf_free(&buf);

            return 1;
        }

        if (fclose(out) != 0) {
            perror("fclose");
            bytebuf_free(&buf);

            return 1;
        }
    }

    if (disasm) {
        if (write_disasm(&buf, asm_path, asm_fmt) != 0) {
            bytebuf_free(&buf);

            return 1;
        }
    }

    if (execute) {
        RegState regs;
        RegState *regs_ptr = debug ? &regs : NULL;

        if (execute_bytes(&buf, regs_ptr) != 0) {
            bytebuf_free(&buf);
            
            return 1;
        }

        if (debug) {
            print_regs(&regs);
        }
    }

    bytebuf_free(&buf);
    return 0;
}