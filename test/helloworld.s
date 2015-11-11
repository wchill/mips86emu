.data
hellostring:    .ascii "Hello World!\n\0"

.text
.globl main
main:
    la  $a0, hellostring
    li  $v0, 4
    syscall

    li  $a0, 0
    li  $v0, 17
    syscall
