	.file	"test2.c"
	.section	.rodata
.LC0:
	.string	"a = %d"
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
	movl	$0, -8(%rbp)
	cmpl	$0, -8(%rbp)
	je	.L2
	movl	$1, -4(%rbp)
	jmp	.L3
.L2:
	cmpl	$9, -8(%rbp)
	jg	.L4
	movl	$2, -4(%rbp)
	jmp	.L3
.L4:
	movl	$3, -4(%rbp)
.L3:
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.7.0"
	.section	.note.GNU-stack,"",@progbits
