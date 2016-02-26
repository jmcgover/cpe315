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
        mov r1, #1      @ Just the number 1
        mov r2, #0      @ Just the number 0
        mov r3, #7     @ Counter
        mov r7, #0
domath:
        add r7, #1
        add r7, r1
        add r7, r7, r1
        add r7, r7, #1
        sub r7, r7, #1
        sub r7, r7, r1
        sub r7, r1
        sub r7, #1

        add r7, #1
        sub r3, #1
        cmp r3, #0
        bgt domath
return:
        mov r0, r7
        pop {r7, pc}
