        .arch armv6
        .fpu softvfp
        .code   16
        .file   "fib.c"
        .text
        .align  2
        .global main
        .code   16
        .thumb_func
        .type   main, %function
main:
        push {r7, lr}
        mov r1, #10
        mov r2, #3
        sub r0, r1, r2
        pop {r7, pc}
