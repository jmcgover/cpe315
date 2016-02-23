    .global main
main:
        push {r7, lr}
        mov r1, #4
        mov r2, #3
        add r0, r1, r2
        pop {r7, pc}
