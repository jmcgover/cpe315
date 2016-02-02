.syntax unified

@ Jeff McGovern
@ Lab 3 - Shift-and-Add
@ Monday 2/1/16

.data
str_add_pre:    .asciz "COUNT_ADD_PRE\n"
str_add:        .asciz "COUNT_ADD_WHILE\n"
str_add_post:   .asciz "COUNT_ADD_POST\n"
.text
@-----myadd-----------------------------
@ r0: a, sum
@ r1: b, a_bit
@ r2: b_bit
@ r3: c_in/scratch3
@ r4: a
@ r5: b
@ r6: mask
@ r7: scratch7
    .global myadd
myadd:
    push {r4,r5,r6,r7,lr}
    @ move a and b to my registers
    mov r4, r0
    mov r5, r1
    @ initialize sum, c_in, and mask
    mov r0, #0          @ sum  = 0x00
    mov r3, #0          @ c_in = 0
    mov r6, #1          @ mask = 0x01
while:
    beq return
    ands r1, r4, r6     @ a_bit = a & mask
    ands r2, r5, r6     @ b_bit = b & mask

    mov r7, r1          @ scratch = a_bit
    eors r7, r7, r2     @ scracth = a_bit ^ b_bit
    eors r7, r7, r3     @ scratch = a_bit ^ b_bit ^ c_in
    orrs r0, r0, r7     @ sum  = sum | scratch

    @ CARRY BIT
    ands r7, r2, r3     @ scratch7 = b_bit & c_in
    ands r3, r1, r3     @ scratch3 = a_bit & c_in
    orrs r7, r3, r7     @ scratch7 = (b_bit & c_in) | (a_bit & c_in)
    ands r3, r1, r2     @ scratch3 = a_bit & b_bit
    @ scratch3 = (a_bit & b_bit) | (b_bit & c_in) | (a_bit & c_in)
    orrs r3, r3, r7
    lsls r3, r3, #1     @ c_in = scratch3 << 1

    lsls r6, r6, #1     @ mask <<= 1
    b while
return:
    @ r0 is already sum
    pop {r4,r5,r6,r7,pc}
