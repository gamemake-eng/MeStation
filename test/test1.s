.section .text
.global __start
__start:
	#exit
	move $a0, $zero
	li $v0, 5058
	syscall

.section .data
