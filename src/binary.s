	.file	"binary.c"
	.text
	.globl	shift_mantissa_left_one
	.type	shift_mantissa_left_one, @function
shift_mantissa_left_one:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%rdi, -72(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-72(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-72(%rbp), %rax
	movzwl	12(%rax), %eax
	shrw	$6, %ax
	movzwl	%ax, %eax
	andl	$1, %eax
	movl	%eax, -52(%rbp)
	cmpl	$0, -52(%rbp)
	jne	.L2
	movl	$0, -60(%rbp)
	movl	$0, -56(%rbp)
	jmp	.L3
.L4:
	movq	-72(%rbp), %rax
	movl	-56(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,4), %eax
	leal	(%rax,%rax), %edx
	movl	-56(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	movl	-56(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %edx
	movl	-60(%rbp), %eax
	orl	%eax, %edx
	movl	-56(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	movq	-72(%rbp), %rax
	movl	-56(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,4), %eax
	shrl	$31, %eax
	movl	%eax, -60(%rbp)
	addl	$1, -56(%rbp)
.L3:
	cmpl	$2, -56(%rbp)
	jle	.L4
	movq	-72(%rbp), %rax
	movzwl	12(%rax), %eax
	addl	%eax, %eax
	movw	%ax, -36(%rbp)
	movzwl	-36(%rbp), %eax
	movl	%eax, %edx
	movl	-60(%rbp), %eax
	orl	%edx, %eax
	movw	%ax, -36(%rbp)
	jmp	.L5
.L2:
	movzbl	-33(%rbp), %eax
	andl	$127, %eax
	orl	$2, %eax
	andl	$127, %eax
	movl	%eax, %edx
	movzbl	-33(%rbp), %eax
	andl	$-128, %eax
	orl	%edx, %eax
	movb	%al, -33(%rbp)
.L5:
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L7
	call	__stack_chk_fail@PLT
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	shift_mantissa_left_one, .-shift_mantissa_left_one
	.globl	shift_mantissa_right_one
	.type	shift_mantissa_right_one, @function
shift_mantissa_right_one:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%rdi, -72(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-72(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-72(%rbp), %rax
	movzwl	12(%rax), %eax
	movzwl	%ax, %eax
	andl	$1, %eax
	movl	%eax, -56(%rbp)
	movq	-72(%rbp), %rax
	movzwl	12(%rax), %eax
	shrw	%ax
	movw	%ax, -36(%rbp)
	movl	$2, -52(%rbp)
	jmp	.L9
.L10:
	movq	-72(%rbp), %rax
	movl	-52(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,4), %eax
	shrl	%eax
	movl	%eax, %edx
	movl	-52(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	movl	-52(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	movl	-56(%rbp), %edx
	sall	$31, %edx
	orl	%eax, %edx
	movl	-52(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	movq	-72(%rbp), %rax
	movl	-52(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,4), %eax
	andl	$1, %eax
	movl	%eax, -56(%rbp)
	subl	$1, -52(%rbp)
.L9:
	cmpl	$0, -52(%rbp)
	jns	.L10
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L12
	call	__stack_chk_fail@PLT
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	shift_mantissa_right_one, .-shift_mantissa_right_one
	.globl	shift_mantissa_left
	.type	shift_mantissa_left, @function
shift_mantissa_left:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -56(%rbp)
	movl	%esi, -60(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-56(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	jmp	.L14
.L15:
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	shift_mantissa_left_one
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
.L14:
	movl	-60(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -60(%rbp)
	testl	%eax, %eax
	jne	.L15
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L17
	call	__stack_chk_fail@PLT
.L17:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	shift_mantissa_left, .-shift_mantissa_left
	.globl	shift_mantissa_right
	.type	shift_mantissa_right, @function
shift_mantissa_right:
.LFB3:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -56(%rbp)
	movl	%esi, -60(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-56(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	jmp	.L19
.L20:
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	shift_mantissa_right_one
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
.L19:
	movl	-60(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -60(%rbp)
	testl	%eax, %eax
	jne	.L20
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L22
	call	__stack_chk_fail@PLT
.L22:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	shift_mantissa_right, .-shift_mantissa_right
	.globl	decimal_xor
	.type	decimal_xor, @function
decimal_xor:
.LFB4:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-72(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movl	$0, -52(%rbp)
	jmp	.L24
.L25:
	movl	-52(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %ecx
	movq	-80(%rbp), %rax
	movl	-52(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,4), %eax
	xorl	%eax, %ecx
	movl	%ecx, %edx
	movl	-52(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	addl	$1, -52(%rbp)
.L24:
	cmpl	$2, -52(%rbp)
	jle	.L25
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L27
	call	__stack_chk_fail@PLT
.L27:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	decimal_xor, .-decimal_xor
	.globl	decimal_and
	.type	decimal_and, @function
decimal_and:
.LFB5:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-72(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movl	$0, -52(%rbp)
	jmp	.L29
.L30:
	movl	-52(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %ecx
	movq	-80(%rbp), %rax
	movl	-52(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,4), %eax
	andl	%eax, %ecx
	movl	%ecx, %edx
	movl	-52(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	addl	$1, -52(%rbp)
.L29:
	cmpl	$2, -52(%rbp)
	jle	.L30
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L32
	call	__stack_chk_fail@PLT
.L32:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	decimal_and, .-decimal_and
	.globl	twos_complement
	.type	twos_complement, @function
twos_complement:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movq	%rdi, -88(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-88(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movl	$0, -68(%rbp)
	jmp	.L34
.L35:
	movl	-68(%rbp), %eax
	cltq
	movl	-64(%rbp,%rax,4), %eax
	notl	%eax
	movl	%eax, %edx
	movl	-68(%rbp), %eax
	cltq
	movl	%edx, -64(%rbp,%rax,4)
	addl	$1, -68(%rbp)
.L34:
	cmpl	$2, -68(%rbp)
	jle	.L35
	movl	$0, %eax
	call	get_one
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	-64(%rbp), %rdi
	movq	-56(%rbp), %rsi
	movq	%rdx, %rcx
	movq	%rax, %rdx
	call	add_mantisses
	movq	%rax, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movq	-64(%rbp), %rax
	movq	-56(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L37
	call	__stack_chk_fail@PLT
.L37:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	twos_complement, .-twos_complement
	.globl	get_one
	.type	get_one, @function
get_one:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$0, -32(%rbp)
	movq	$0, -24(%rbp)
	movl	$1, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L40
	call	__stack_chk_fail@PLT
.L40:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	get_one, .-get_one
	.globl	decimal_is_zero
	.type	decimal_is_zero, @function
decimal_is_zero:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	$1, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L42
.L43:
	movq	-24(%rbp), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,4), %eax
	testl	%eax, %eax
	sete	%al
	movzbl	%al, %eax
	movl	-8(%rbp), %edx
	imull	%edx, %eax
	movl	%eax, -8(%rbp)
	addl	$1, -4(%rbp)
.L42:
	cmpl	$2, -4(%rbp)
	jle	.L43
	movl	-8(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	decimal_is_zero, .-decimal_is_zero
	.globl	add_mantisses
	.type	add_mantisses, @function
add_mantisses:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%rdx, %rax
	movq	%rcx, %rdx
	movq	%rdi, -64(%rbp)
	movq	%rsi, -56(%rbp)
	movq	%rax, -80(%rbp)
	movq	%rdx, -72(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-64(%rbp), %rax
	movq	-56(%rbp), %rdx
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	jmp	.L46
.L47:
	leaq	-80(%rbp), %rdx
	leaq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	decimal_and
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	shift_mantissa_left_one
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	leaq	-80(%rbp), %rdx
	leaq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	decimal_xor
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rax, -80(%rbp)
	movq	%rdx, -72(%rbp)
.L46:
	leaq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	decimal_is_zero
	testl	%eax, %eax
	je	.L47
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L49
	call	__stack_chk_fail@PLT
.L49:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	add_mantisses, .-add_mantisses
	.section	.rodata
.LC0:
	.string	"%d %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$0, -48(%rbp)
	movq	$0, -40(%rbp)
	movl	$-1, -48(%rbp)
	movl	$1, -44(%rbp)
	movl	$0, -52(%rbp)
	jmp	.L51
.L52:
	movl	$0, %eax
	call	get_one
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	-48(%rbp), %rdi
	movq	-40(%rbp), %rsi
	movq	%rdx, %rcx
	movq	%rax, %rdx
	call	add_mantisses
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	addl	$1, -52(%rbp)
.L51:
	cmpl	$99999, -52(%rbp)
	jle	.L52
	movl	-44(%rbp), %edx
	movl	-48(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L54
	call	__stack_chk_fail@PLT
.L54:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
