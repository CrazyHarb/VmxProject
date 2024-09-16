#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/printk.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/smp.h>
#include "asm_function_x64.h"
#include "vmcsfield.h"
#include "rdmsr_struct.h"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("L");

typedef union _Segment_Selector {
  unsigned short all;
  struct {
    unsigned short rpl : 2;  //!< Requested Privilege Level
    unsigned short ti : 1;   //!< Table Indicator
    unsigned short index : 13;
  } fields;
}__attribute__((packed)) Segment_Selector, *PSegment_Selector;

typedef union _Vmx_Regment_Descriptor_AccessRight {
  uint32_t all;
  struct {
    uint32_t type : 4;        //!< [0:3]
    uint32_t system : 1;      //!< [4]
    uint32_t dpl : 2;         //!< [5:6]
    uint32_t present : 1;     //!< [7]
    uint32_t reserved1 : 4;   //!< [8:11]
    uint32_t avl : 1;         //!< [12]
    uint32_t l : 1;           //!< [13] Reserved (except for CS) 64-bit mode
    uint32_t db : 1;          //!< [14]
    uint32_t gran : 1;        //!< [15]
    uint32_t unusable : 1;    //!< [16] Segment unusable
    uint32_t reserved2 : 15;  //!< [17:31]
  } fields;
}__attribute__((packed)) Vmx_Regment_Descriptor_AccessRight, *PVmx_Regment_Descriptor_AccessRight;

typedef union _Segment_Descriptor {
  uint64_t all;
  struct {
    uint64_t limit_low : 16;
    uint64_t base_low : 16;
    uint64_t base_mid : 8;
    uint64_t type : 4;
    uint64_t system : 1;
    uint64_t dpl : 2;
    uint64_t present : 1;
    uint64_t limit_high : 4;
    uint64_t avl : 1;
    uint64_t l : 1;  //!< 64-bit code segment (IA-32e mode only)
    uint64_t db : 1;
    uint64_t gran : 1;
    uint64_t base_high : 8;
  } fields;
}__attribute__((packed)) Segment_Descriptor, *PSegment_Descriptor;

typedef struct _Segment_DesctiptorX64 {
  Segment_Descriptor descriptor;
  uint32_t base_upper32;
  uint32_t reserved;
}__attribute__((packed)) Segment_DesctiptorX64, *PSegment_DesctiptorX64;

uint32_t WhisperGetSegmentAccessRight(unsigned short segment_selector) {
  Vmx_Regment_Descriptor_AccessRight access_right = {};
  if (segment_selector) {
    Segment_Selector ss = {segment_selector};
    uint32_t native_access_right = (uint32_t)Asm_LoadAccessRightsByte(ss.all);
    native_access_right >>= 8;
    access_right.all = native_access_right;
    access_right.fields.reserved1 = 0;
    access_right.fields.reserved2 = 0;
    access_right.fields.unusable = false;
  } else {
    access_right.fields.unusable = true;
  }
  return access_right.all;
}

Segment_Descriptor *WhisperGetSegmentDescriptor(uint64_t descriptor_table_base, unsigned short segment_selector) {
  Segment_Selector ss = {segment_selector};
  return (Segment_Descriptor *)(
      descriptor_table_base + ss.fields.index * sizeof(Segment_Descriptor));
}

uint64_t WhisperGetSegmentBaseByDescriptor(Segment_Descriptor *segment_descriptor) {
  // Calculate a 32bit base address
  uint64_t base_high = segment_descriptor->fields.base_high << (6 * 4);
  uint64_t base_middle = segment_descriptor->fields.base_mid << (4 * 4);
  uint64_t base_low = segment_descriptor->fields.base_low;
  uint64_t base = (base_high | base_middle | base_low) & 0xFFFFFFFF;
  // Get upper 32bit of the base address if needed
  if (!segment_descriptor->fields.system) {
    Segment_DesctiptorX64 *desc64 = (Segment_DesctiptorX64 *)(segment_descriptor);
    uint64_t base_upper32 = desc64->base_upper32;
    base |= (base_upper32 << 32);
  }
  return base;
}

uint64_t WhisperGetSegmentBase(uint64_t gdt_base, unsigned short segment_selector) {
  const Segment_Selector ss = {segment_selector};
  if (!ss.all) {
    return 0;
  }

  if (ss.fields.ti) {
    Segment_Descriptor *local_segment_descriptor =
        WhisperGetSegmentDescriptor(gdt_base, Asm_readldtr());
    uint64_t ldt_base =
        WhisperGetSegmentBaseByDescriptor(local_segment_descriptor);
    Segment_Descriptor *segment_descriptor =
        WhisperGetSegmentDescriptor(ldt_base, segment_selector);
    return WhisperGetSegmentBaseByDescriptor(segment_descriptor);
  } else {
    Segment_Descriptor *segment_descriptor =
        WhisperGetSegmentDescriptor(gdt_base, segment_selector);
    return WhisperGetSegmentBaseByDescriptor(segment_descriptor);
  }
}

typedef struct _Vm_Control_Structure {
  unsigned long revision_identifier;
  unsigned long vmx_abort_indicator;
  unsigned long data[0]; 
}__attribute__((packed)) Vm_Control_Structure, *PVm_Control_Structure;

typedef struct _Idtr {
  unsigned short limit;
  uint64_t base;
}__attribute__((packed)) Idtr, *PIdtr;

typedef struct _Gdtr {
  unsigned short limit;
  uint64_t base;
}__attribute__((packed)) Gdtr, *PGdtr;

void InitCrX(void) {

    unsigned long val0 = read_cr0();
    unsigned long long cr0fixed0 = __rdmsr(Msr_kIa32VmxCr0Fixed0);
    unsigned long long cr0fixed1 = __rdmsr(Msr_kIa32VmxCr0Fixed1);

    val0 &= cr0fixed1;
    val0 |= cr0fixed0;

    write_cr0(val0);


    unsigned long val4 = __read_cr4();
    unsigned long long cr4fixed0 = __rdmsr(Msr_kIa32VmxCr4Fixed0);
    unsigned long long cr4fixed1 = __rdmsr(Msr_kIa32VmxCr4Fixed1);

    val4 &= cr4fixed1;
    val4 |= cr4fixed0;

    __write_cr4(val4);
}

bool InitVmxon(void) {

    bool l_bool_result = false;
    unsigned long long val = __rdmsr(Msr_kIa32VmxBasic);

    PVm_Control_Structure vmxonbuf = kmalloc(4096, GFP_KERNEL);

    if (vmxonbuf)
    {
        vmxonbuf->revision_identifier = val & 0x7FFFFFFF;

        l_bool_result = !Asm_Vmxon((uint64_t)virt_to_phys(vmxonbuf));
    }

    return l_bool_result;
}

bool InitVmcs(void) {
  bool l_bool_result = false;
    unsigned long long val = __rdmsr(Msr_kIa32VmxBasic);

    PVm_Control_Structure vmcsbuf = kmalloc(4096, GFP_KERNEL);

    if (vmcsbuf)
    {
        vmcsbuf->revision_identifier = val & 0x7FFFFFFF;

        l_bool_result = !Asm_VmClear((uint64_t)virt_to_phys(vmcsbuf)) && !Asm_Vmptrld((uint64_t)virt_to_phys(vmcsbuf));
    }

    return l_bool_result;
}

uint32_t WhisperAdjustControlValue(uint64_t msr, uint32_t requested_value) {

  unsigned long long val = __rdmsr(msr);

  requested_value &= val >> 32;
  requested_value |= val & 0xFFFFFFF;

  return requested_value;
}

bool SetupVMCS(void *guest_rsp, void *guest_rip, void *host_rip) {
   Gdtr gdtr = {};
    Asm_SGDT(&gdtr);
    
    Idtr idtr = {};
    Asm_SIDT(&idtr);
    
    Asm_vmxWrite(VmcsField_kGuestEsSelector, Asm_readES());
    Asm_vmxWrite(VmcsField_kGuestCsSelector, Asm_readCS());
    Asm_vmxWrite(VmcsField_kGuestSsSelector, Asm_readSS());
    Asm_vmxWrite(VmcsField_kGuestDsSelector, Asm_readDS());
    Asm_vmxWrite(VmcsField_kGuestFsSelector, Asm_readFS());
    Asm_vmxWrite(VmcsField_kGuestGsSelector, Asm_readGS());
    Asm_vmxWrite(VmcsField_kGuestLdtrSelector, Asm_readldtr());
    Asm_vmxWrite(VmcsField_kGuestTrSelector, Asm_readTR());
    
    Asm_vmxWrite(VmcsField_kGuestIa32Debugctl, __rdmsr(Msr_kIa32Debugctl));
    
    Asm_vmxWrite(VmcsField_kGuestEsLimit, __segmentlimit(Asm_readES()));
    Asm_vmxWrite(VmcsField_kGuestCsLimit, __segmentlimit(Asm_readCS()));
    Asm_vmxWrite(VmcsField_kGuestSsLimit, __segmentlimit(Asm_readSS()));
    Asm_vmxWrite(VmcsField_kGuestDsLimit, __segmentlimit(Asm_readDS()));
    Asm_vmxWrite(VmcsField_kGuestFsLimit, __segmentlimit(Asm_readFS()));
    Asm_vmxWrite(VmcsField_kGuestGsLimit, __segmentlimit(Asm_readGS()));
    Asm_vmxWrite(VmcsField_kGuestLdtrLimit, __segmentlimit(Asm_readldtr()));
    Asm_vmxWrite(VmcsField_kGuestTrLimit, __segmentlimit(Asm_readTR()));
    Asm_vmxWrite(VmcsField_kGuestGdtrLimit, gdtr.limit);
    Asm_vmxWrite(VmcsField_kGuestIdtrLimit, idtr.limit);
    Asm_vmxWrite(VmcsField_kGuestEsArBytes, WhisperGetSegmentAccessRight(Asm_readES()));
    Asm_vmxWrite(VmcsField_kGuestCsArBytes, WhisperGetSegmentAccessRight(Asm_readCS()));
    Asm_vmxWrite(VmcsField_kGuestSsArBytes, WhisperGetSegmentAccessRight(Asm_readSS()));
    Asm_vmxWrite(VmcsField_kGuestDsArBytes, WhisperGetSegmentAccessRight(Asm_readDS()));
    Asm_vmxWrite(VmcsField_kGuestFsArBytes, WhisperGetSegmentAccessRight(Asm_readFS()));
    Asm_vmxWrite(VmcsField_kGuestGsArBytes, WhisperGetSegmentAccessRight(Asm_readGS()));
    Asm_vmxWrite(VmcsField_kGuestLdtrArBytes, WhisperGetSegmentAccessRight(Asm_readldtr()));
    Asm_vmxWrite(VmcsField_kGuestTrArBytes, WhisperGetSegmentAccessRight(Asm_readTR()));
    
    Asm_vmxWrite(VmcsField_kGuestSysenterCs, __rdmsr(Msr_kIa32SysenterCs));
    Asm_vmxWrite(VmcsField_kCr0GuestHostMask, 0);
    Asm_vmxWrite(VmcsField_kCr4GuestHostMask, 0);
    Asm_vmxWrite(VmcsField_kCr0ReadShadow, 0);
    Asm_vmxWrite(VmcsField_kCr4ReadShadow, 0);
    
    Asm_vmxWrite(VmcsField_kGuestCr0, read_cr0());
    Asm_vmxWrite(VmcsField_kGuestCr3, __read_cr3());
    Asm_vmxWrite(VmcsField_kGuestCr4, __read_cr4());
    Asm_vmxWrite(VmcsField_kGuestEsBase, 0);
    Asm_vmxWrite(VmcsField_kGuestCsBase, 0);
    Asm_vmxWrite(VmcsField_kGuestSsBase, 0);
    Asm_vmxWrite(VmcsField_kGuestDsBase, 0);
    
    Asm_vmxWrite(VmcsField_kGuestFsBase,  __rdmsr(Msr_kIa32FsBase));
    Asm_vmxWrite(VmcsField_kGuestGsBase,  __rdmsr(Msr_kIa32GsBase));
    
    Asm_vmxWrite(VmcsField_kGuestLdtrBase, WhisperGetSegmentBase(gdtr.base, Asm_readldtr()));
    Asm_vmxWrite(VmcsField_kGuestTrBase, WhisperGetSegmentBase(gdtr.base, Asm_readTR()));
    Asm_vmxWrite(VmcsField_kGuestGdtrBase, gdtr.base);
    Asm_vmxWrite(VmcsField_kGuestIdtrBase, idtr.base);
    Asm_vmxWrite(VmcsField_kGuestDr7, Asm_readDr7());
    Asm_vmxWrite(VmcsField_kGuestRflags, Asm_readEflags());
    
    Asm_vmxWrite(VmcsField_kGuestSysenterEsp, __rdmsr(Msr_kIa32SysenterEsp));
    Asm_vmxWrite(VmcsField_kGuestSysenterEip, __rdmsr(Msr_kIa32SysenterEip));

    Asm_vmxWrite(VmcsField_kHostEsSelector, Asm_readES() & 0xf8);
    Asm_vmxWrite(VmcsField_kHostCsSelector, Asm_readCS() & 0xf8);
    Asm_vmxWrite(VmcsField_kHostSsSelector, Asm_readSS() & 0xf8);
    Asm_vmxWrite(VmcsField_kHostDsSelector, Asm_readDS() & 0xf8);
    Asm_vmxWrite(VmcsField_kHostFsSelector, Asm_readFS() & 0xf8);
    Asm_vmxWrite(VmcsField_kHostGsSelector, Asm_readGS() & 0xf8);
    Asm_vmxWrite(VmcsField_kHostTrSelector, Asm_readTR() & 0xf8);

    Asm_vmxWrite(VmcsField_kHostIa32SysenterCs, __rdmsr(Msr_kIa32SysenterCs));

    Asm_vmxWrite(VmcsField_kHostFsBase,  __rdmsr(Msr_kIa32FsBase));
    Asm_vmxWrite(VmcsField_kHostGsBase,  __rdmsr(Msr_kIa32GsBase));
    Asm_vmxWrite(VmcsField_kHostIa32SysenterEsp, __rdmsr(Msr_kIa32SysenterEsp));
    Asm_vmxWrite(VmcsField_kHostIa32SysenterEip, __rdmsr(Msr_kIa32SysenterEip));

    Asm_vmxWrite(VmcsField_kHostCr0, read_cr0());
    Asm_vmxWrite(VmcsField_kHostCr3, __read_cr3());
    Asm_vmxWrite(VmcsField_kHostCr4, __read_cr4());
    Asm_vmxWrite(VmcsField_kHostTrBase, WhisperGetSegmentBase(gdtr.base, Asm_readTR()));
    Asm_vmxWrite(VmcsField_kHostGdtrBase, gdtr.base);
    Asm_vmxWrite(VmcsField_kHostIdtrBase, idtr.base);

    Asm_vmxWrite(VmcsField_kVmcsLinkPointer, ~((uint64_t)0u));
    bool l_bool_useTrueMsr = (__rdmsr(Msr_kIa32VmxBasic) >> 55) & 0x1;

    Asm_vmxWrite(VmcsField_kPinBasedVmExecControl,  WhisperAdjustControlValue((l_bool_useTrueMsr) ? Msr_kIa32VmxTruePinbasedCtls: Msr_kIa32VmxPinbasedCtls, 0));
    Asm_vmxWrite(VmcsField_kCpuBasedVmExecControl, WhisperAdjustControlValue((l_bool_useTrueMsr) ? Msr_kIa32VmxTrueProcBasedCtls: Msr_kIa32VmxProcBasedCtls, 0));
    Asm_vmxWrite(VmcsField_kExceptionBitmap, 0);
    Asm_vmxWrite(VmcsField_kVmExitControls, WhisperAdjustControlValue((l_bool_useTrueMsr) ? Msr_kIa32VmxTrueExitCtls: Msr_kIa32VmxExitCtls, 1 << 9));
    Asm_vmxWrite(VmcsField_kVmEntryControls, WhisperAdjustControlValue((l_bool_useTrueMsr) ? Msr_kIa32VmxTrueEntryCtls: Msr_kIa32VmxEntryCtls, (1 << 2) | (1 << 9)));
    Asm_vmxWrite(VmcsField_kSecondaryVmExecControl, WhisperAdjustControlValue(Msr_kIa32VmxProcBasedCtls2, 0));

    Asm_vmxWrite(VmcsField_kGuestRsp, (uint64_t)guest_rsp);
    Asm_vmxWrite(VmcsField_kGuestRip, (uint64_t)guest_rip);

    char *host_rsp = (char*)kmalloc(4096 * 3, GFP_KERNEL);
    Asm_vmxWrite(VmcsField_kHostRsp, (uint64_t)(host_rsp + 4096 * 3));
    Asm_vmxWrite(VmcsField_kHostRip, (uint64_t)host_rip);

    Asm_launch();

    printk(KERN_INFO "Asm_launch run failed\n");
    return false;
}

bool InitializeVM(void *guest_rsp, void *guest_rip, void *host_rip) {
    if( InitVmxon() && InitVmcs() && SetupVMCS(guest_rsp, guest_rip, host_rip)) {
      return true;
    } else {
      return false;
    }
}

int whisper_init(void)
{	
    unsigned int eax = 0x1, ebx = 0, ecx = 0, edx = 0;
    __cpuid(&eax, &ebx, &ecx, &edx);

    unsigned long long val = __rdmsr(Msr_kIa32FeatureControl);
    bool isSupported = (ecx & (1 << 5)) && (val & (1 << 2));
    if (isSupported)
    {
       val |= 1;
       __wrmsr(Msr_kIa32FeatureControl, val, val >> 32);

       InitCrX();
       isSupported = Asm_init((void*)InitializeVM);
       if (isSupported)
       {
         printk(KERN_INFO "Asm_init run success\n");
       } else {
         printk(KERN_INFO "Asm_init run failed\n");
       }
    }
    
    if (!isSupported)
    {
        printk(KERN_INFO "%s\n", "unspported");
    }
    
    return 0;
}

void whisper_exit(void)
{
   
}

module_init(whisper_init);
module_exit(whisper_exit);