.section .text
.global __start
__start:
	#exit
	li $v0, 1
	li $a0, 9
	syscall

.section .data
