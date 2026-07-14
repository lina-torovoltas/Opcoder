#define _POSIX_C_SOURCE 200809L

#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



static void bytebuf_init(ByteBuf *buf) {
    buf -> bytes = NULL;
    buf -> len = 0;
    buf -> cap = 0;
}


static int bytebuf_add(ByteBuf *buf, uint8_t byte) {
    if (buf -> len == buf -> cap) {

        if (buf -> cap > SIZE_MAX / 2)

            return -1;

        size_t new_cap = buf -> cap ? buf -> cap * 2 : 64;

        uint8_t *new_bytes = realloc(buf -> bytes, new_cap);

        if (!new_bytes)

            return -1;

        buf -> bytes = new_bytes;
        buf -> cap = new_cap;
    }

    buf -> bytes[buf -> len++] = byte;

    return 0;
}


void bytebuf_free(ByteBuf *buf) {
    free(buf -> bytes);

    buf -> bytes = NULL;
    buf -> len = 0;
    buf -> cap = 0;
}


typedef struct {
    char data[4096];
    size_t len;
} BitBuf;


static void bitbuf_reset(BitBuf *buf) {
    buf -> len = 0;
}


static int bitbuf_flush(BitBuf *bits, ByteBuf *out) {
    if (bits -> len == 0)

        return 0;

    if (bits -> len % 8 != 0)

        return -1;

    for (size_t i = 0; i < bits -> len; i += 8) {
        uint8_t byte = 0;

        for (int b = 0; b < 8; b++) {
            byte <<= 1;
            byte |= bits -> data[i + b] - '0';
        }

        if (bytebuf_add(out, byte) != 0)

            return -1;
    }


    bitbuf_reset(bits);

    return 0;
}


static int is_hex(char c) {
    return isdigit((unsigned char)c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}


static void rstrip(char *s) {
    size_t n = strlen(s);

    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || isspace((unsigned char)s[n - 1]))) {
        s[--n] = '\0';
    }
}


static void strip_comment(char *line) {
    char *semi = strchr(line, ';');

    if (semi)
        *semi = '\0';
}


static int parse_line(char *line, int line_no, ByteBuf *out) {
    strip_comment(line);
    rstrip(line);

    BitBuf bits;
    bitbuf_reset(&bits);

    char *saveptr = NULL;
    char *tok = strtok_r(line, " \t", &saveptr);

    while (tok) {
        size_t tlen = strlen(tok);

        if (tlen == 0) {
            tok = strtok_r(NULL, " \t", &saveptr);
            continue;
        }

        char last = tok[tlen - 1];

        if (last == 'h' || last == 'H') {
            if (bitbuf_flush(&bits, out) != 0) {
                fprintf(stderr, "Line %d: bit sequence not multiple of 8\n", line_no);

                return -1;
            }

            if (tlen != 3 || !is_hex(tok[0]) || !is_hex(tok[1])) {
                fprintf(stderr, "Line %d: invalid hex token '%s'\n", line_no, tok);

                return -1;
            }

            char hex[3] = {
                tok[0],
                tok[1],
                0
            };

            uint8_t byte = (uint8_t)strtol(hex, NULL, 16);

            if (bytebuf_add(out, byte) != 0) {
                fprintf(stderr, "Line %d: out of memory\n", line_no);

                return -1;
            }

        } else if (last == 'b' || last == 'B') {
            size_t nbits = tlen - 1;

            if (nbits == 0) {
                fprintf(stderr, "Line %d: empty bit token '%s'\n", line_no, tok);

                return -1;
            }

            for (size_t i = 0; i < nbits; i++) {
                if (tok[i] != '0' && tok[i] != '1') {
                    fprintf(stderr, "Line %d: invalid bit token '%s'\n", line_no, tok);

                    return -1;
                }
            }

            if (bits.len + nbits > sizeof(bits.data)) {
                fprintf(stderr, "Line %d: bit sequence too long\n", line_no);

                return -1;
            }

            memcpy(bits.data + bits.len, tok, nbits);
            bits.len += nbits;

        } else {
            if (bitbuf_flush(&bits, out) != 0) {
                fprintf(stderr, "Line %d: bit sequence not multiple of 8\n", line_no);

                return -1;
            }

            fprintf(stderr, "Line %d: unknown token '%s'\n", line_no, tok);

            return -1;
        }

        tok = strtok_r(NULL, " \t", &saveptr);
    }

    if (bitbuf_flush(&bits, out) != 0) {
        fprintf(stderr, "Line %d: bit sequence not multiple of 8\n", line_no);

        return -1;
    }

    return 0;
}


int parse_opc_file(const char *path, ByteBuf *out) {
    FILE *f = fopen(path, "r");

    if (!f) {
        fprintf(stderr, "Cannot open '%s'\n", path);

        return -1;
    }

    bytebuf_init(out);

    char line[4096];
    int line_no = 0;
    int status = 0;

    while (fgets(line, sizeof(line), f)) {
        line_no++;

        char *p = line;

        while (*p && isspace((unsigned char)*p))
            p++;

        if (*p == '\0')
            continue;

        if (parse_line(line, line_no, out) != 0) {
            status = -1;
            break;
        }
    }

    fclose(f);

    if (status != 0)
        bytebuf_free(out);

    return status;
}