	.file	"test6.c"
	.section	.rodata
.LC4:
	.string	"Values AFTER %f, %f\n"
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
	movl	.LC0(%rip), %eax
	movl	%eax, -4(%rbp)
	jmp	.L2
.L6:
	movss	-4(%rbp), %xmm0
	ucomiss	.LC1(%rip), %xmm0
	jbe	.L9
.L8:
	movl	.LC2(%rip), %eax
	movl	%eax, -8(%rbp)
	movss	-4(%rbp), %xmm0
	subss	-8(%rbp), %xmm0
	movss	%xmm0, -4(%rbp)
	jmp	.L2
.L9:
	jmp	.L5
.L2:
	movss	-4(%rbp), %xmm0
	xorps	%xmm1, %xmm1
	ucomiss	%xmm1, %xmm0
	ja	.L6
.L5:
	movss	-8(%rbp), %xmm1
	cvtps2pd	%xmm1, %xmm1
	movss	-4(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC4, %edi
	movl	$2, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1086324736
	.align 4
.LC1:
	.long	1084227584
	.align 4
.LC2:
	.long	1082130432
	.ident	"GCC: (GNU) 4.7.0"
	.section	.note.GNU-stack,"",@progbits
