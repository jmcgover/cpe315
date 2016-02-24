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
        cmp r1, r2
        blt answer
        mov r0, #2
        b return
answer:
        mov r0, #7
return:
        pop {r7, pc}

