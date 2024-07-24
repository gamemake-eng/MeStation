.org 0x1E8480
.section .text
.global __start
__start:
	#exit
	li $v0, 1
	li $a0, 9
	syscall

.section .data
text: .asciiz "Hello"
