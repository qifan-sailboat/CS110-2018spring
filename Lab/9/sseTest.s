	.file	"sseTest.c"
	.section	.rodata
.LC5:
	.string	"|%g %g| * |%g %g| = |%g %g|\n"
.LC6:
	.string	"|%g %g|   |%g %g|   |%g %g|\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB499:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$464, %rsp
	movl	%edi, -436(%rbp)
	movq	%rsi, -448(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -112(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -104(%rbp)
	movsd	.LC2(%rip), %xmm0
	movsd	%xmm0, -96(%rbp)
	movsd	.LC3(%rip), %xmm0
	movsd	%xmm0, -88(%rbp)
	movsd	.LC2(%rip), %xmm0
	movsd	%xmm0, -80(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -72(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -64(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -56(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -48(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -40(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -32(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, -424(%rbp)
	movq	-424(%rbp), %rax
	# m128d c1 = _mm_loadu_pd( C+0 );
	movupd	(%rax), %xmm0
	movaps	%xmm0, -352(%rbp)
	leaq	-48(%rbp), %rax
	addq	$16, %rax
	movq	%rax, -376(%rbp)
	movq	-376(%rbp), %rax
	#_m128d c2 = _mm_loadu_pd( C+2 );
	movupd	(%rax), %xmm0
	movaps	%xmm0, -336(%rbp)
	movl	$0, -428(%rbp)
	jmp	.L4

.L14:
	movl	-428(%rbp), %eax
	cltq
	salq	$4, %rax
	movq	%rax, %rdx
	leaq	-112(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -408(%rbp)
	movq	-408(%rbp), %rax
	#__m128d a  = _mm_loadu_pd( A+i*2 );
	movupd	(%rax), %xmm0
	movaps	%xmm0, -304(%rbp)
	movl	-428(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	leaq	-80(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -400(%rbp)
	movq	-400(%rbp), %rax
	movsd	(%rax), %xmm0
	movsd	%xmm0, -360(%rbp)
	movsd	-360(%rbp), %xmm0
	unpcklpd	%xmm0, %xmm0
	movaps	%xmm0, -288(%rbp)
	movl	-428(%rbp), %eax
	cltq
	addq	$2, %rax
	leaq	0(,%rax,8), %rdx
	leaq	-80(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -392(%rbp)
	movq	-392(%rbp), %rax
	movsd	(%rax), %xmm0
	movsd	%xmm0, -368(%rbp)
	movsd	-368(%rbp), %xmm0
	unpcklpd	%xmm0, %xmm0
	movaps	%xmm0, -272(%rbp)
	movapd	-304(%rbp), %xmm0
	movaps	%xmm0, -256(%rbp)
	movapd	-288(%rbp), %xmm0
	movaps	%xmm0, -160(%rbp)
	movapd	-256(%rbp), %xmm0
	# _mm_mul_pd
	mulpd	-160(%rbp), %xmm0
	movapd	-352(%rbp), %xmm1
	movaps	%xmm1, -240(%rbp)
	movaps	%xmm0, -176(%rbp)
	movapd	-240(%rbp), %xmm0
	# _mm_add_pd
	addpd	-176(%rbp), %xmm0
	movaps	%xmm0, -352(%rbp)
	movapd	-304(%rbp), %xmm0
	movaps	%xmm0, -224(%rbp)
	movapd	-272(%rbp), %xmm0
	movaps	%xmm0, -192(%rbp)
	movapd	-224(%rbp), %xmm0
	# _mm_mul_pd
	mulpd	-192(%rbp), %xmm0
	movapd	-336(%rbp), %xmm1
	movaps	%xmm1, -320(%rbp)
	movaps	%xmm0, -208(%rbp)
	movapd	-320(%rbp), %xmm0
	# _mm_add_pd
	addpd	-208(%rbp), %xmm0
	movaps	%xmm0, -336(%rbp)
	addl	$1, -428(%rbp)
.L4:
	cmpl	$1, -428(%rbp)
	jle	.L14
	leaq	-48(%rbp), %rax
	movq	%rax, -384(%rbp)
	movapd	-352(%rbp), %xmm0
	movaps	%xmm0, -128(%rbp)
	movq	-384(%rbp), %rax
	movapd	-128(%rbp), %xmm0
	movups	%xmm0, (%rax)
	leaq	-48(%rbp), %rax
	addq	$16, %rax
	movq	%rax, -416(%rbp)
	movapd	-336(%rbp), %xmm0
	movaps	%xmm0, -144(%rbp)
	movq	-416(%rbp), %rax
	movapd	-144(%rbp), %xmm0
	movups	%xmm0, (%rax)
	movsd	-32(%rbp), %xmm4
	movsd	-48(%rbp), %xmm3
	movsd	-64(%rbp), %xmm2
	movsd	-80(%rbp), %xmm1
	movsd	-96(%rbp), %xmm0
	movq	-112(%rbp), %rax
	movapd	%xmm4, %xmm5
	movapd	%xmm3, %xmm4
	movapd	%xmm2, %xmm3
	movapd	%xmm1, %xmm2
	movapd	%xmm0, %xmm1
	movq	%rax, -456(%rbp)
	movsd	-456(%rbp), %xmm0
	movl	$.LC5, %edi
	movl	$6, %eax
	call	printf
	movsd	-24(%rbp), %xmm4
	movsd	-40(%rbp), %xmm3
	movsd	-56(%rbp), %xmm2
	movsd	-72(%rbp), %xmm1
	movsd	-88(%rbp), %xmm0
	movq	-104(%rbp), %rax
	movapd	%xmm4, %xmm5
	movapd	%xmm3, %xmm4
	movapd	%xmm2, %xmm3
	movapd	%xmm1, %xmm2
	movapd	%xmm0, %xmm1
	movq	%rax, -456(%rbp)
	movsd	-456(%rbp), %xmm0
	movl	$.LC6, %edi
	movl	$6, %eax
	call	printf
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L16
	call	__stack_chk_fail
.L16:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE499:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC1:
	.long	0
	.long	1073741824
	.align 8
.LC2:
	.long	0
	.long	1074266112
	.align 8
.LC3:
	.long	0
	.long	1074790400
	.ident	"GCC: (Ubuntu 5.4.1-2ubuntu1~16.04) 5.4.1 20160904"
	.section	.note.GNU-stack,"",@progbits
