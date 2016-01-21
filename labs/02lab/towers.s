@ Jeff McGovern
@ Lab 2 - Towers of Hanoi
@ Thursday 1/21/16

.data
prompt:         .asciz "Enter the number of disks to be moved: "
scan_int_fmt:   .asciz "%d"
test_numDiscs:  .asciz "NumDiscs: %d\n"
fuck:           .asciz "Fuck\n"


.balign 4
scan_int_val: .word scan_int_val

.text

@----------------------------------------
    .global towers
towers:
    push {lr}
    mov r0, #0
    pop {pc}

@----------------------------------------
    .global main
main:
    push {r4, r5, r6, ip, lr}

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
    mov r4, r0
    mov r5, r1
    mov r6, r2
    bl towers @ Call towers function

    @mov r0, #0      @ return 0
    pop {r4, r5, r6, ip, pc}    @ Exit program
