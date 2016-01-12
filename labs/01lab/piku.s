@ Jeff McGovern
@ Lab 1 - Pis and ARM Assembly
@ Monday 1/11/16

    .syntax unified
    @ --------
    .global main
main:
    @ Stack the return address (lr) in additiong to a dummy register (ip) to
    @ keep the stack 8-byte aligned.
    push    {ip,lr}

    @ Load the argument and perform the call. This is like 'printf("...")' in C.
    ldr r0, =line1  @ Load line1
    bl printf       @ Print line1
    ldr r0, =line2  @ Load line2
    bl printf       @ Print line2
    ldr r0, =line3  @ Load line3
    bl printf       @ Print line3

    @ Exit from 'main'. This is like 'return 0' in C.
    mov     r0, #0  @ return 0

    @ Pop the dummy ip to reverse our alignment fix and pop the original lr
    @ value directly into pc - the Program Counter - to return.
    pop     {ip, pc}

    @ --------
    @ Data for the printf call. The GNU assemblr's ".asciz" firective
    @ automatically adds a NULL character termination.
line1:
    .asciz "my raspberry pi's ---\n"
line2:
    .asciz "a veritable army ---\n"
line3:
    .asciz "now speak assembly\n"
