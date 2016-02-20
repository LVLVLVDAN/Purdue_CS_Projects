	.file	"test3.c"
	.section	.rodata
.LC0:
	.string	"now i = 1"
.LC1:
	.string	"now i!=1"
.LC2:
	.string	"now i = 2"
.LC3:
	.string	"%d %d %d \n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$1, -4(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L2
	movl	$.LC0, %edi
	call	puts
	movl	$10, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	-4(%rbp), %edx
	addl	%edx, %eax
	jmp	.L3
.L2:
	movl	$.LC1, %edi
	call	puts
	cmpl	$2, -4(%rbp)
	jne	.L4
	movl	$10, -8(%rbp)
	movl	$10, -12(%rbp)
	movl	$.LC2, %edi
	call	puts
	movl	-12(%rbp), %eax
	movl	-8(%rbp), %edx
	addl	%eax, %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	jmp	.L3
.L4:
	movl	$10, -8(%rbp)
	movl	-16(%rbp), %ecx
	movl	-12(%rbp), %edx
	movl	-8(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	movl	-4(%rbp), %eax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.7.0"
	.section	.note.GNU-stack,"",@progbits
