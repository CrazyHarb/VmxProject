#include <linux/module.h>
#include <linux/kernel.h>
#include "vmcsfield.h"
#include "asm_function_x64.h"
#include "processdata.h"
#include "rdmsr_struct.h"

#define VMWARE_CMD_GETVERSION       10
#define VMWARE_CMD_GETHZ            45
#define VMWARE_CMD_GETVCPU_INFO     68
#define VMWARE_CMD_STEALCLOCK       91

#define VMWARE_CMD_ABSPOINTER_DATA  39
#define VMWARE_CMD_ABSPOINTER_STATUS    40
#define VMWARE_CMD_ABSPOINTER_COMMAND   41
#define VMWARE_CMD_ABSPOINTER_RESTRICT  86

bool Handler(Guest_Context *guest_context) {
    uint64_t exitReason = Asm_VmRead(VmcsField_kVmExitReason);
    if (exitReason == VmxExitReason_kCpuid)
    {
        if (guest_context->stack->gp_regs.ax == 0)
        {
            guest_context->stack->gp_regs.bx = 0x68747541;
			guest_context->stack->gp_regs.dx = 0x69746e65;
			guest_context->stack->gp_regs.cx = 0x444d4163;
            
            guest_context->stack->result_reg.rip = guest_context->stack->result_reg.rip + Asm_VmRead(VmcsField_kVmExitInstructionLen);
            return true;
        } else if (guest_context->stack->gp_regs.ax != 'whis')
		{
			__cpuid((unsigned int *)&guest_context->stack->gp_regs.ax, (unsigned int *)&guest_context->stack->gp_regs.bx, 
					(unsigned int *)&guest_context->stack->gp_regs.cx, (unsigned int *)&guest_context->stack->gp_regs.dx);
			 guest_context->stack->result_reg.rip = guest_context->stack->result_reg.rip + Asm_VmRead(VmcsField_kVmExitInstructionLen);
            return true;
		} else {
			printk(KERN_INFO "cpuid stop 0x%llx\n", guest_context->stack->gp_regs.ax);
			return false;
		}
    } else  if (exitReason == VmxExitReason_kMsrWrite)
    {
		bool transfer_to_vmcs = false;
		bool l_bool_tryRun = false;
		uint64_t vmcs_field = 0;
		switch ( guest_context->stack->gp_regs.cx) {
		  case Msr_kIa32SysenterCs:
		    vmcs_field = VmcsField_kGuestSysenterCs;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32SysenterEsp:
		    vmcs_field = VmcsField_kGuestSysenterEsp;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32SysenterEip:
		    vmcs_field = VmcsField_kGuestSysenterEip;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32Debugctl:
		    vmcs_field = VmcsField_kGuestIa32Debugctl;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32GsBase:
		    vmcs_field = VmcsField_kGuestGsBase;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32FsBase:
		    vmcs_field = VmcsField_kGuestFsBase;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  default:
		    break;
		}

		uint64_t val = (guest_context->stack->gp_regs.ax & 0xFFFFFFFF) | ((guest_context->stack->gp_regs.dx & 0xFFFFFFFF) << 32);

		if (transfer_to_vmcs)
		{
			uint64_t writeresult = Asm_vmxWrite(vmcs_field, val);
		} else {
			__wrmsr(guest_context->stack->gp_regs.cx, guest_context->stack->gp_regs.ax, guest_context->stack->gp_regs.dx);
			l_bool_tryRun = true;
		}

		// continue if write successfully!
		if (l_bool_tryRun)
		{
			guest_context->stack->result_reg.rip = guest_context->stack->result_reg.rip + Asm_VmRead(VmcsField_kVmExitInstructionLen);
			return true;
		}
	} else  if (exitReason == VmxExitReason_kMsrRead)
    {
		bool transfer_to_vmcs = false;
		bool l_bool_tryRun = false;
		uint64_t vmcs_field = 0;
		switch ( guest_context->stack->gp_regs.cx) {
		  case Msr_kIa32SysenterCs:
		    vmcs_field = VmcsField_kGuestSysenterCs;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32SysenterEsp:
		    vmcs_field = VmcsField_kGuestSysenterEsp;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32SysenterEip:
		    vmcs_field = VmcsField_kGuestSysenterEip;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32Debugctl:
		    vmcs_field = VmcsField_kGuestIa32Debugctl;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32GsBase:
		    vmcs_field = VmcsField_kGuestGsBase;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  case Msr_kIa32FsBase:
		    vmcs_field = VmcsField_kGuestFsBase;
		    transfer_to_vmcs = true;
			l_bool_tryRun = true;
		    break;
		  default:
		    break;
		}

		uint64_t val = (guest_context->stack->gp_regs.ax & 0xFFFFFFFF) | ((guest_context->stack->gp_regs.dx & 0xFFFFFFFF) << 32);

		if (transfer_to_vmcs)
		{
			uint64_t readresult = Asm_VmRead(vmcs_field);

			guest_context->stack->gp_regs.ax = readresult & 0xFFFFFFFF;
			readresult >>= 32;
			guest_context->stack->gp_regs.dx = readresult & 0xFFFFFFFF;
		} else {
			uint64_t readresult = __rdmsr(guest_context->stack->gp_regs.cx);
			
			guest_context->stack->gp_regs.ax = readresult & 0xFFFFFFFF;
			readresult >>= 32;
			guest_context->stack->gp_regs.dx = readresult & 0xFFFFFFFF;

			l_bool_tryRun = true;
		}

		// continue if write successfully!
		if (l_bool_tryRun)
		{
			guest_context->stack->result_reg.rip = guest_context->stack->result_reg.rip + Asm_VmRead(VmcsField_kVmExitInstructionLen);
			return true;
		}
	} else  if (exitReason == VmxExitReason_kVmcall) {
		if (guest_context->stack->gp_regs.cx == VMWARE_CMD_ABSPOINTER_DATA || 
			guest_context->stack->gp_regs.cx == VMWARE_CMD_ABSPOINTER_STATUS ||
			guest_context->stack->gp_regs.cx == VMWARE_CMD_ABSPOINTER_COMMAND ||
			guest_context->stack->gp_regs.cx == VMWARE_CMD_ABSPOINTER_RESTRICT ||
			guest_context->stack->gp_regs.cx == VMWARE_CMD_STEALCLOCK ||
			guest_context->stack->gp_regs.cx == VMWARE_CMD_GETVCPU_INFO ||
			guest_context->stack->gp_regs.cx == VMWARE_CMD_GETHZ ||
			guest_context->stack->gp_regs.cx == VMWARE_CMD_GETVERSION)
		{
			Asm_VmCallForVmware(&(guest_context->stack->gp_regs));
			
		} else {
			guest_context->flag_reg.fields.cf = 1;
			guest_context->flag_reg.fields.zf = 0;
		}
		guest_context->stack->result_reg.rip = guest_context->stack->result_reg.rip + Asm_VmRead(VmcsField_kVmExitInstructionLen);
		return true;
	}
    
	printk(KERN_INFO "reason:0x%llx stop\n", exitReason);
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
	stack->result_reg.cr3 = Asm_VmRead(VmcsField_kGuestCr3);


	guest_context.vm_continue = Handler(&guest_context);

	if (!guest_context.vm_continue)
	{

	} else {
		Asm_vmxWrite(VmcsField_kGuestRip, stack->result_reg.rip);
		Asm_vmxWrite(VmcsField_kGuestRflags, guest_context.flag_reg.all);
	}

	return guest_context.vm_continue;
}