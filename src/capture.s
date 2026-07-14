.intel_syntax noprefix

.text
.global exec_captreg
.type exec_captreg, @function

exec_captreg:
    push rsi
    call rdi

    mov r11, [rsp]

    mov [r11+0],   rax
    mov [r11+8],   rbx
    mov [r11+16],  rcx
    mov [r11+24],  rdx
    mov [r11+32],  rsi
    mov [r11+40],  rdi
    mov [r11+48],  rbp
    mov [r11+64],  r8
    mov [r11+72],  r9
    mov [r11+80],  r10
    mov [r11+88],  r12
    mov [r11+96],  r13
    mov [r11+104], r14
    mov [r11+112], r15

    mov rax, rsp
    add rax, 8
    mov [r11+56], rax

    pop rsi
    ret

.size exec_captreg, . - exec_captreg

.section .note.GNU-stack,"",@progbits
