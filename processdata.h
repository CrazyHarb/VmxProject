#include <linux/types.h>

typedef struct _Whisper_Result_Registers {
   	uint64_t rip;
  	uint64_t cs;
	uint64_t eflags;
  	uint64_t rsp;
  	uint64_t ss;
	uint64_t cr3;
}__attribute__((packed)) Whisper_Result_Registers, *PWhisper_Result_Registers;

typedef struct _Whisper_GpRegisters {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t di;
	uint64_t si;
	uint64_t bp;
	uint64_t sp;
	uint64_t bx;
	uint64_t dx;
	uint64_t cx;
	uint64_t ax;
}__attribute__((packed)) Whisper_GpRegisters, *PWhisper_GpRegisters;

typedef struct _Whisper_XmmRegisters {
	unsigned __int128  xmm0;
	unsigned __int128  xmm1;
	unsigned __int128  xmm2;
	unsigned __int128  xmm3;
	unsigned __int128  xmm4;
	unsigned __int128  xmm5;
  	unsigned __int128  xmm6;
	unsigned __int128  xmm7;
	unsigned __int128  xmm8;
	unsigned __int128  xmm9;
	unsigned __int128  xmm10;
	unsigned __int128  xmm11;
  	unsigned __int128  xmm12;
	unsigned __int128  xmm13;
	unsigned __int128  xmm14;
	unsigned __int128  xmm15;
}__attribute__((packed)) Whisper_XmmRegisters, *PWhisper_XmmRegisters;

typedef union _Whisper_FlagRegister {
	uint64_t all;
	struct {
		uint64_t cf : 1;          //!< [0] Carry flag
		uint64_t reserved1 : 1;   //!< [1] Always 1
		uint64_t pf : 1;          //!< [2] Parity flag
		uint64_t reserved2 : 1;   //!< [3] Always 0
		uint64_t af : 1;          //!< [4] Borrow flag
		uint64_t reserved3 : 1;   //!< [5] Always 0
		uint64_t zf : 1;          //!< [6] Zero flag
		uint64_t sf : 1;          //!< [7] Sign flag
		uint64_t tf : 1;          //!< [8] Trap flag
		uint64_t intf : 1;        //!< [9] Interrupt flag
		uint64_t df : 1;          //!< [10] Direction flag
		uint64_t of : 1;          //!< [11] Overflow flag
		uint64_t iopl : 2;        //!< [12:13] I/O privilege level
		uint64_t nt : 1;          //!< [14] Nested task flag
		uint64_t reserved4 : 1;   //!< [15] Always 0
		uint64_t rf : 1;          //!< [16] Resume flag
		uint64_t vm : 1;          //!< [17] Virtual 8086 mode
		uint64_t ac : 1;          //!< [18] Alignment check
		uint64_t vif : 1;         //!< [19] Virtual interrupt flag
		uint64_t vip : 1;         //!< [20] Virtual interrupt pending
		uint64_t id : 1;          //!< [21] Identification flag
		uint64_t reserved5 : 10;  //!< [22:31] Always 0
	} fields;
}__attribute__((packed)) Whisper_FlagRegister, *PWhisper_FlagRegister;

typedef struct _Whisper_Call_Stack {
  	Whisper_XmmRegisters xmm_regs;    //  16 * 16 = 0x100 = 256
	Whisper_GpRegisters gp_regs;      //  16 * 8 = 0x80 = 128
  	Whisper_FlagRegister flag_reg;    //  8
  	Whisper_Result_Registers result_reg;   // 2 * 8 = 0x10 = 32
}__attribute__((packed)) Whisper_Call_Stack, *PWhisper_Call_Stack;

typedef struct _Guest_Context {
	Whisper_Call_Stack* stack;
	Whisper_FlagRegister flag_reg;
	uint64_t ip;
	uint64_t cr8;
	bool vm_continue;
}__attribute__((packed)) Guest_Context, *PGuest_Context;

