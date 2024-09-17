
.text

.global Asm_Vmxon,Asm_VmClear,Asm_Vmptrld, Asm_launch
.global Asm_vmxWrite
.global Asm_readES, Asm_readSS, Asm_readDS, Asm_readCS, Asm_readFS, Asm_readGS
.global Asm_readTR, Asm_readldtr, Asm_init
.global __segmentlimit, Asm_LoadAccessRightsByte, Asm_SGDT, Asm_SIDT, Asm_LGDT, Asm_LIDT
.global Asm_readDr7, Asm_readEflags
.global Asm_ReadCr8, Asm_VmRead

Asm_launch:
	vmlaunch
	ret

Asm_VmRead:
	subq $0x8, %rsp
	movq $0, (%rsp)
	movq %rdi, %rax
	vmread %rax, (%rsp)
	setz %al
	setb %cl
	adc %cl, %al
	cmp $0x0, %al
	jnz vmerror
	movq (%rsp), %rax
	addq $0x8, %rsp
	ret
vmerror:
	addq $0x8, %rsp
	ret

Asm_vmxWrite:
    mov %rdi, %rax
    mov %rsi, %rdi
    vmwrite %rdi, %rax
    setz %al
    setb %cl
    adc  %cl, %al
    movzx %al, %rax
    ret

Asm_Vmxon:
	subq $0x10, %rsp
	movq %rdi, (%rsp)

	vmxon (%rsp)
	addq $0x10, %rsp
	
    setz %al
	setb %cl
	adc %cl, %al
	movzx %al,%rax
	ret

Asm_VmClear:
	push %rdi
	vmclear (%rsp)
	addq $8, %rsp
	setz %al
	setb %cl
	adc  %cl,%al
	movzx %al, %rax
	ret

Asm_Vmptrld:
	push %rdi
	vmptrld (%rsp)
	addq $8, %rsp
	setz %al
	setb %cl
	adc  %cl, %al
	movzx %al, %rax
	ret

Asm_readES:
        mov %es, %ax        
        ret

Asm_readCS:
        mov %cs, %ax
        ret

Asm_readSS:
        mov %ss, %ax
        ret

Asm_readDS:
        mov %ds, %ax
        ret

Asm_readFS:
        mov %fs, %ax
        ret

Asm_readGS:
        mov %gs, %ax
        ret

Asm_readTR:
        str %ax
        ret

Asm_readldtr:
        sldt %ax
        ret

__segmentlimit:
        xorq %rax, %rax
        movzx %di, %rdi
        lsl %rdi,%rax
        ret
        
Asm_LoadAccessRightsByte:
        lar %rdi, %rax
        ret
        
Asm_SGDT:
        sgdt (%rdi)
        ret

Asm_SIDT:
        sidt (%rdi)
        ret

Asm_LGDT:
        lgdt (%rdi)
        ret

Asm_LIDT:
        lidt (%rdi)
        ret
        
Asm_readDr7:
        movq %dr7, %rax
        ret
        
Asm_readEflags:
        pushfq
        pop %rax
        ret

Asm_ReadCr8:
	movq %cr8, %rax
	ret

.macro Asm_pushad
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rbx
	pushq $0xFFFFFFFFFFFFFFFF
	pushq %rbp
	pushq %rsi
	pushq %rdi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
.endm

.macro Asm_popad
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdi
	popq %rsi
	popq %rbp
	addq $8,%rsp
	popq %rbx
	popq %rdx
	popq %rcx
	popq %rax
.endm

.macro Asm_saveXmm
	subq $0x100, %rsp                #  256 bytes
	movaps %xmm0, (%rsp)
	movaps %xmm1, 16(%rsp)
	movaps %xmm2, 32(%rsp)
	movaps %xmm3, 48(%rsp)
	movaps %xmm4, 64(%rsp)
	movaps %xmm5, 80(%rsp)
	movaps %xmm6, 96(%rsp)
	movaps %xmm7, 112(%rsp)
	movaps %xmm8, 128(%rsp)
	movaps %xmm9, 144(%rsp)
	movaps %xmm10, 160(%rsp)
	movaps %xmm11, 176(%rsp)
	movaps %xmm12, 192(%rsp)
	movaps %xmm13, 208(%rsp)
	movaps %xmm14, 224(%rsp)
	movaps %xmm15, 240(%rsp)
.endm

.macro Asm_storeXmm
	movaps (%rsp),%xmm0
	movaps 16(%rsp),%xmm1
	movaps 32(%rsp),%xmm2
	movaps 48(%rsp),%xmm3
	movaps 64(%rsp),%xmm4
	movaps 80(%rsp),%xmm5
	movaps 96(%rsp),%xmm6
	movaps 112(%rsp),%xmm7
	movaps 128(%rsp),%xmm8
	movaps 144(%rsp),%xmm9
	movaps 160(%rsp),%xmm10
	movaps 176(%rsp),%xmm11
	movaps 192(%rsp),%xmm12
	movaps 208(%rsp),%xmm13
	movaps 224(%rsp),%xmm14
	movaps 240(%rsp),%xmm15
	addq $0x100, %rsp
.endm

Asm_init:
		pushfq
		Asm_pushad
		movq %rdi, %rax
    	movq %rsp, %rdi
		leaq guest_run, %rsi
		leaq Asm_VmmEntryPoint, %rdx

		call *%rax

		Asm_popad
		popfq
		xorq %rax,%rax
		ret

guest_run:
		Asm_popad
		popfq
		xorq %rax,%rax

		inc %rax
		ret

Asm_VmmEntryPoint:
	subq $0x18, %rsp  # "movaps needs 16 bytes aligned"  --huoji (key08.com)
	pushfq      # save eflags
	Asm_pushad  # save regs
	Asm_saveXmm # save xmm regs

	movq %rsp, %rdi
	call WhisperExitHandler

	test %rax,%rax
	jz call_vmmOff

	Asm_storeXmm # restore xmm
	Asm_popad    # restore regs
	popfq        # restore eflags
	vmresume     # vmresume error should handler error

call_vmmOff:
	Asm_storeXmm # restore xmm
	Asm_popad    # restore regs
	popfq        # restore eflags
	vmxoff
	movq 0(%rsp), %rax
	movq 8(%rsp), %rsp

	jmp *%rax
	
	             