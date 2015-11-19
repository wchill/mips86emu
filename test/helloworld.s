.data
hellostring:    .ascii "Hello World!\n\0"
saved_ret_pc:   .word 0

.text
.globl main
main:
    sw $ra, saved_ret_pc

    la  $a0, hellostring
    li  $v0, 4
    syscall

    li  $a0, 1
    jal test
    li  $a0, 2
    jal test
    li  $a0, 3
    jal test
    li  $a0, 4
    jal test

    li  $a0, 0
    li  $v0, 17
    syscall

test:
    li $v0, 1
    syscall
    jr $ra
