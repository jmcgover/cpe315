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
        mov r7, #7
        mov r3, sp
        mov r4, r3
        stm r3!,{r7}
        mov r7, #11
        ldm r4!,{r7}
        mov r0, r7
        pop {r7, pc}


