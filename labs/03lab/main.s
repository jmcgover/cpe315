.syntax unified

@ Jeff McGovern
@ Lab 3 - Shift-and-Add
@ Monday 2/1/16

.arch armv6
.fpu vfp

.data
str_prompt_1:   .asciz "Enter Number 1: "
str_prompt_2:   .asciz "Enter Number 2: "
fmt_scan_int:   .asciz "%d"
fmt_scan_chr:   .asciz " %c"
fmt_product:    .asciz "Product is: %d\n"
str_again:      .asciz "Again? "

fmt_int_test:   .asciz "int  val: %d\n"
fmt_chr_test:   .asciz "char val: %c\n"

.balign 4
int_val: .word int_val

.balign 4
chr_val: .word chr_val

.align
chr_affirm: .byte 'y'

.text
@-----main------------------------------
@ r0:
@ r1:
@ r2:
@ r3:
@ r4: multiplicand
@ r5: multiplier
@ r6: product
@ r7: again input
    .global main
main:
    push {ip, lr}

loop:
@ Request input 1
    ldr r0, =str_prompt_1
    bl printf

    ldr r0, =fmt_scan_int
    ldr r1, =int_val
    bl scanf
    ldr r4, =int_val
    ldr r4, [r4]

@ Request input 2
    ldr r0, =str_prompt_2
    bl printf

    ldr r0, =fmt_scan_int
    ldr r1, =int_val
    bl scanf
    ldr r5, =int_val
    ldr r5, [r5]

@ Perform Multiply
    mov r0, r4
    mov r1, r5
    bl mymult
    mov r6, r0

@ Print Answer
    ldr r0, =fmt_product
    mov r1, r6
    bl printf

@ Perform Again?
    ldr r0, =str_again
    bl printf

    ldr r0, =fmt_scan_chr
    ldr r1, =chr_val
    bl scanf
    ldr r7, =chr_val
    ldrb r7, [r7]

    mov r0, #0
    mov r0, r7
    ldr r1, =chr_affirm
    ldrb r1, [r1]
    cmp r0, r1
    beq loop

@ return
    mov r0, #0
    pop {ip, pc}    @ Exit program
