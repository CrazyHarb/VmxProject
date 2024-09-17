#include <linux/module.h>
#include <linux/kernel.h>
#include "vmcsfield.h"
#include "asm_function_x64.h"
#include "processdata.h"

bool Handler(Guest_Context *guest_context) {
    uint64_t exitReason = Asm_VmRead(VmcsField_kVmExitReason);
    if (exitReason == VmxExitReason_kCpuid)
    {
        if (guest_context->stack->gp_regs.ax == 0)
        {
            guest_context->stack->gp_regs.bx = 'ekaF';
            guest_context->stack->gp_regs.dx = 'etnI';
            guest_context->stack->gp_regs.cx = 'upCl';
            
            guest_context->stack->result_reg.rip = guest_context->stack->result_reg.rip + Asm_VmRead(VmcsField_kVmExitInstructionLen);
            return true;
        }
    }
    
    return false;
 }

bool WhisperExitHandler(Whisper_Call_Stack *stack) {

	uint64_t guest_cr8 = Asm_ReadCr8();
	Guest_Context guest_context = {
		.stack = stack,
		.flag_reg = {Asm_VmRead(VmcsField_kGuestRflags)},
		.ip = Asm_VmRead(VmcsField_kGuestRip),
		.cr8 = guest_cr8,
		.vm_continue = false };

	stack->result_reg.rsp = Asm_VmRead(VmcsField_kGuestRsp);
	stack->result_reg.cs = Asm_VmRead(VmcsField_kGuestCsSelector);
	stack->result_reg.rip = guest_context.ip;
	stack->result_reg.ss = Asm_VmRead(VmcsField_kGuestSsSelector);
	stack->result_reg.eflags =Asm_VmRead(VmcsField_kGuestRflags);


	guest_context.vm_continue = Handler(&guest_context);

	if (!guest_context.vm_continue)
	{

	} else {
		Asm_vmxWrite(VmcsField_kGuestRip, stack->result_reg.rip);
	}

	return guest_context.vm_continue;
}