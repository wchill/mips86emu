.text
.globl main
main:
	li $v0, 4
	li $v1, 2

	div $t0, $v0, $v1
	#bne $t0, 2, fail
	#mfhi $t0
	#bne $t0, 0, fail

	.data
fm:	.asciiz "Failed test\n"
	.text
fail:	li $v0, 4	# syscall 4 (print_str)
	la $a0, fm
	syscall
	li $v0, 10	# syscall 10, (exit)
	syscall
