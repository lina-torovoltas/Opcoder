#define _POSIX_C_SOURCE 200809L

#include "execution.h"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>



static const int prot = PROT_READ | PROT_WRITE;
static const int flags = MAP_PRIVATE | MAP_ANONYMOUS;


typedef void (*VoidFunc)(void);


extern void exec_captreg(VoidFunc code, RegState *out);


int execute_bytes(const ByteBuf *buf, RegState *regs_out) {
    if (buf -> len == 0) {
        fprintf(stderr, "Error: nothing to execute (0 bytes parsed)\n");
        
        return -1;
    }

    void *mem = mmap(NULL, buf -> len, prot, flags, -1, 0);

    if (mem == MAP_FAILED) {
        perror("mmap");

        return -1;
    }

    memcpy(mem, buf -> bytes, buf -> len);

    if (mprotect(mem, buf -> len, PROT_READ | PROT_EXEC) != 0) {
        perror("mprotect");
        munmap(mem, buf -> len);

        return -1;
    }

    if (regs_out) {
        exec_captreg((VoidFunc)mem, regs_out);
    } else {
        VoidFunc func = (VoidFunc)mem;
        func();
    }

    if (munmap(mem, buf -> len) != 0) {
        perror("munmap");
        
        return -1;
    }

    return 0;
}