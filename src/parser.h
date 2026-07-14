#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *bytes;
    size_t   len;
    size_t   cap;
} ByteBuf;


int parse_opc_file(const char *path, ByteBuf *out);

void bytebuf_free(ByteBuf *buf);

#endif