
.text

.global Asm_Vmxon,Asm_VmClear,Asm_Vmptrld, Asm_launch
.global Asm_vmxWrite
.global Asm_readES, Asm_readSS, Asm_readDS, Asm_readCS, Asm_readFS, Asm_readGS
.global Asm_readTR, Asm_readldtr, Asm_init
.global __segmentlimit, Asm_LoadAccessRightsByte, Asm_SGDT, Asm_SIDT, Asm_LGDT, Asm_LIDT
.global Asm_readDr7, Asm_readEflags

Asm_launch:
	vmlaunch
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

Asm_init:
		pushfq
		Asm_pushad
		movq %rdi, %rax
    	movq %rsp, %rdi
		leaq guest_run, %rsi
		leaq host_run, %rdx

		call *%rax

guest_run:
		cpuid
		Asm_popad
		popfq
		xorq %rax,%rax
		ret

host_run:
		vmxoff
		Asm_popad
		popfq
		xorq %rax,%rax

		inc %rax
		ret
