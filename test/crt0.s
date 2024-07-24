.section .text
.global __start
__start:
	jal main
	li $v0, 0
	syscall
