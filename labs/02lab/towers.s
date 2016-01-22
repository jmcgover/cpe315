@ Jeff McGovern
@ Lab 2 - Towers of Hanoi
@ Thursday 1/21/16

.data
prompt:         .asciz "Enter the number of disks to be moved: "
scan_int_fmt:   .asciz "%d"
print_fmt:      .asciz "Move from peg %d to peg %d\n"
from_peg_fmt:   .asciz "\n%d discs moved from peg %d "
to_peg_fmt:     .asciz "to peg %d "
in_steps_fmt:   .asciz "in %d steps.\n"

params_fmt:     .asciz "numDiscs: %d start: %d goal: %d\n"
test_numDiscs:  .asciz "NumDiscs: %d\n"
fuck:           .asciz "Fuck\n"
str_if:         .asciz "if\n"
str_else:       .asciz "else\n"
str_towers:     .asciz "towers\n"


.balign 4
scan_int_val: .word scan_int_val

.text
@-----print-----------------------------
@ r0: from
@ r1: to
    .global print
print:
    push {r4,lr}
    @ Move from and to over a register for printf call
    mov r2, r1
    mov r1, r0
    ldr r0, =print_fmt
    bl printf

    @ return
    pop {r4,pc}

@-----towers----------------------------
@ r4: numDiscs
@ r5: start
@ r6: goal
@ r7: steps
@ r8: peg

    .global towers
towers:
    push {r4,r5,r6,r7,r8,lr}

    @ Move parameters to caller save registers
    mov r4, r0
    mov r5, r1
    mov r6, r2

    @ if (numDiscs < 2)
    cmp r4,#2

    blt if
    @ numDiscs >= 2

    @ peg = 6 - start - goal;
    mov r8, #6
    sub r8, r5
    sub r8, r6

    @ numDiscs -= 1;
    sub r4, #1

    @ steps  = towers(numDiscs-1, start, peg);
    mov r0, r4
    mov r1, r5
    mov r2, r8
    bl towers
    mov r7, r0

    @ steps += towers(1, start, goal);
    mov r0, #1
    mov r1, r5
    mov r2, r6
    bl towers
    add r7, r0

    @ steps += towers(numDiscs-1, peg, goal);
    mov r0, r4
    mov r1, r8
    mov r2, r6
    bl towers
    add r7, r0

    @ return steps
    mov r0, r7
    pop {r4,r5,r6,r7,r8,pc}
if:
    @ numDiscs < 2
    @ print(start, goal)
    mov r0, r5
    mov r1, r6
    bl print

    @ return 1
    mov r0, #1
    pop {r4,r5,r6,r7,r8,pc}

@-----main------------------------------
@ r4: numDiscs
@ r5: start
@ r6: goal
@ r7: steps
    .global main
main:
    push {r4, r5, r6, r7, ip, lr}

    @ Request input from user
    ldr r0, =prompt
    bl printf

    @ Scan in numDiscs and load into r4
    ldr r0, =scan_int_fmt
    ldr r1, =scan_int_val
    bl scanf
    ldr r4, =scan_int_val
    ldr r4, [r4]

    @ Initialize start and goal
    mov r5, #1      @ start = 1
    mov r6, #3      @ goal = 1

    @ Move numDiscs, start, and goal into the arg registers
    mov r0, r4
    mov r1, r5
    mov r2, r6
    bl towers @ Call towers function
    mov r7, r0

    @ Print results
    ldr r0, =from_peg_fmt
    mov r1, r4
    mov r2, r5
    bl printf
    ldr r0, =to_peg_fmt
    mov r1, r6
    bl printf
    ldr r0, =in_steps_fmt
    mov r1, r7
    bl printf

    @ return 0
    mov r0, #0      @ return 0
    pop {r4, r5, r6, r7, ip, pc}    @ Exit program
