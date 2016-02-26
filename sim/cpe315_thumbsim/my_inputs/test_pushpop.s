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
        mov r1, #1
        mov r2, #2
        mov r7, #7
        push {r1, r2, r7}
        pop {r1, r2, r7}
        mov r0, r7
        pop {r7, pc}

