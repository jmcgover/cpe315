@ Jeff McGovern
@ Lab 3 - Shift-and-Add
@ Monday 2/1/16

.data
str_prompt_1:   .asciz "Enter Number 1: "
str_prompt_2:   .asciz "Enter Number 2: "
fmt_scan_int:   .asciz "%d"
fmt_scan_chr:   .asciz " %c"
fmt_product:    .asciz "Product is: %d\n"
str_again:      .asciz "Again? "

.balign 4
int_val: .word int_val

.balign 4
chr_val: .word chr_val

.text
@-----myadd-----------------------------
@ r0: a
@ r1: b
    .global myadd
myadd:
    push {r4,lr}
    @ do add
    @ return
    pop {r4,pc}

@-----mymult-----------------------------
@ r0: a
@ r1: b
    .global mymult
mymult:
    push {r4,lr}
    @ do mult
    @ return
    pop {r4,pc}

@-----main------------------------------
@ r4:
@ r5:
@ r6:
@ r7:
    .global main
main:
    push {ip, lr}
    @ Request input from user
    @ Perform Multiply
    @ Print Answer
    @ Perform Again?
    @ return
    mov r0, #0
    pop {ip, pc}    @ Exit program
