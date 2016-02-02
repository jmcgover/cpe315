.syntax unified

@ Jeff McGovern
@ Lab 3 - Shift-and-Add
@ Monday 2/1/16

.data
fmt_product:    .asciz "MULT: Product is: %d\n"
fmt_reg_test:   .asciz "r%d: %d\n"
fmt_all_reg:    .asciz "r4: %d r5: %d r6: %d\n"
fmt_int_test:   .asciz "int  val: %d\n"
fmt_chr_test:   .asciz "char val: %c\n"

str_mult:       .asciz "COUNT_MULT\n"
str_mult_if:    .asciz "COUNT_MULT_IF\n"
.text
@-----mymult-----------------------------
@ r0: multiplicand, scratch, param prouct
@ r1: multiplier, param multiplicand
@ r2:
@ r3:
@ r4: multiplicand
@ r5: multiplier
@ r6: product
@ r7:
    .global mymult
mymult:
    push {r4,r5,r6,lr}
    mov r4, r0
    mov r5, r1
    mov r6, #0          @ int product = 0;
while:
    cmp r5, #0
    beq return          @ Skip while loop
    lsls r0, r5, #31    @ if (multiplier & LSB_MASK == 1)
    cmp r0, #0          @ If LSB isn't 1, then whole num is 0
    beq after_add       @ Skip the add section
    @ product = product + multiplicand;
    mov r0, r6
    mov r1, r4
    bl myadd
    mov r6, r0
after_add:
    lsls r4, r4, #1     @ multiplicand = multiplicand << 1;
    lsrs r5, r5, #1     @ multiplier = multiplier >> 1;
    b while             @ Start loop over

return:
    mov r0, r6
    pop {r4,r5,r6,pc}
