#include <linux/types.h>

bool Asm_Vmxon(uint64_t a_uint64_address);
bool Asm_VmClear(uint64_t a_voidptr_address);
bool Asm_Vmptrld(uint64_t a_voidptr_address);

uint64_t Asm_vmxWrite(uint64_t a_uint64_index, uint64_t a_uint64_val);

void Asm_SGDT(void *a_voidptr_gdtBuf);
void Asm_SIDT(void *a_voidptr_idtBuf);
void Asm_LGDT(void *a_voidptr_gdtBuf);
void Asm_LIDT(void *a_voidptr_idtBuf);
uint64_t Asm_readTR(void);
uint64_t Asm_readldtr(void);

uint64_t Asm_readCS(void);
uint64_t Asm_readDS(void);
uint64_t Asm_readES(void);
uint64_t Asm_readFS(void);
uint64_t Asm_readGS(void);
uint64_t Asm_readSS(void);

uint64_t Asm_readDr7(void);

uint64_t __segmentlimit(uint64_t a_segment_limit);
uint64_t Asm_LoadAccessRightsByte(uint64_t segment_selector);

uint64_t Asm_readEflags(void);

bool Asm_init(void *func);

void Asm_launch(void);
void Asm_VmCallForVmware(void *regstack);

uint64_t Asm_VmRead(uint64_t a_uint64_index);
uint64_t Asm_ReadCr8(void);