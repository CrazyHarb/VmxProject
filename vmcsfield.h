#include "linux/types.h"

enum VmcsField {
  // 16-Bit Control Field
  VmcsField_kVirtualProcessorId = 0x00000000,
  VmcsField_kPostedInterruptNotification = 0x00000002,
  VmcsField_kEptpIndex = 0x00000004,
  // 16-Bit Guest-State Fields
  VmcsField_kGuestEsSelector = 0x00000800,
  VmcsField_kGuestCsSelector = 0x00000802,
  VmcsField_kGuestSsSelector = 0x00000804,
  VmcsField_kGuestDsSelector = 0x00000806,
  VmcsField_kGuestFsSelector = 0x00000808,
  VmcsField_kGuestGsSelector = 0x0000080a,
  VmcsField_kGuestLdtrSelector = 0x0000080c,
  VmcsField_kGuestTrSelector = 0x0000080e,
  VmcsField_kGuestInterruptStatus = 0x00000810,
  VmcsField_kPmlIndex = 0x00000812,
  // 16-Bit Host-State Fields
  VmcsField_kHostEsSelector = 0x00000c00,
  VmcsField_kHostCsSelector = 0x00000c02,
  VmcsField_kHostSsSelector = 0x00000c04,
  VmcsField_kHostDsSelector = 0x00000c06,
  VmcsField_kHostFsSelector = 0x00000c08,
  VmcsField_kHostGsSelector = 0x00000c0a,
  VmcsField_kHostTrSelector = 0x00000c0c,
  // 64-Bit Control Fields
  VmcsField_kIoBitmapA = 0x00002000,
  VmcsField_kIoBitmapAHigh = 0x00002001,
  VmcsField_kIoBitmapB = 0x00002002,
  VmcsField_kIoBitmapBHigh = 0x00002003,
  VmcsField_kMsrBitmap = 0x00002004,
  VmcsField_kMsrBitmapHigh = 0x00002005,
  VmcsField_kVmExitMsrStoreAddr = 0x00002006,
  VmcsField_kVmExitMsrStoreAddrHigh = 0x00002007,
  VmcsField_kVmExitMsrLoadAddr = 0x00002008,
  VmcsField_kVmExitMsrLoadAddrHigh = 0x00002009,
  VmcsField_kVmEntryMsrLoadAddr = 0x0000200a,
  VmcsField_kVmEntryMsrLoadAddrHigh = 0x0000200b,
  VmcsField_kExecutiveVmcsPointer = 0x0000200c,
  VmcsField_kExecutiveVmcsPointerHigh = 0x0000200d,
  VmcsField_kTscOffset = 0x00002010,
  VmcsField_kTscOffsetHigh = 0x00002011,
  VmcsField_kVirtualApicPageAddr = 0x00002012,
  VmcsField_kVirtualApicPageAddrHigh = 0x00002013,
  VmcsField_kApicAccessAddr = 0x00002014,
  VmcsField_kApicAccessAddrHigh = 0x00002015,
  VmcsField_kEptPointer = 0x0000201a,
  VmcsField_kEptPointerHigh = 0x0000201b,
  VmcsField_kEoiExitBitmap0 = 0x0000201c,
  VmcsField_kEoiExitBitmap0High = 0x0000201d,
  VmcsField_kEoiExitBitmap1 = 0x0000201e,
  VmcsField_kEoiExitBitmap1High = 0x0000201f,
  VmcsField_kEoiExitBitmap2 = 0x00002020,
  VmcsField_kEoiExitBitmap2High = 0x00002021,
  VmcsField_kEoiExitBitmap3 = 0x00002022,
  VmcsField_kEoiExitBitmap3High = 0x00002023,
  VmcsField_kEptpListAddress = 0x00002024,
  VmcsField_kEptpListAddressHigh = 0x00002025,
  VmcsField_kVmreadBitmapAddress = 0x00002026,
  VmcsField_kVmreadBitmapAddressHigh = 0x00002027,
  VmcsField_kVmwriteBitmapAddress = 0x00002028,
  VmcsField_kVmwriteBitmapAddressHigh = 0x00002029,
  VmcsField_kVirtualizationExceptionInfoAddress = 0x0000202a,
  VmcsField_kVirtualizationExceptionInfoAddressHigh = 0x0000202b,
  VmcsField_kXssExitingBitmap = 0x0000202c,
  VmcsField_kXssExitingBitmapHigh = 0x0000202d,
  VmcsField_kEnclsExitingBitmap = 0x0000202e,
  VmcsField_kEnclsExitingBitmapHigh = 0x0000202f,
  VmcsField_kTscMultiplier = 0x00002032,
  VmcsField_kTscMultiplierHigh = 0x00002033,
  // 64-Bit Read-Only Data Field
  VmcsField_kGuestPhysicalAddress = 0x00002400,
  VmcsField_kGuestPhysicalAddressHigh = 0x00002401,
  // 64-Bit Guest-State Fields
  VmcsField_kVmcsLinkPointer = 0x00002800,
  VmcsField_kVmcsLinkPointerHigh = 0x00002801,
  VmcsField_kGuestIa32Debugctl = 0x00002802,
  VmcsField_kGuestIa32DebugctlHigh = 0x00002803,
  VmcsField_kGuestIa32Pat = 0x00002804,
  VmcsField_kGuestIa32PatHigh = 0x00002805,
  VmcsField_kGuestIa32Efer = 0x00002806,
  VmcsField_kGuestIa32EferHigh = 0x00002807,
  VmcsField_kGuestIa32PerfGlobalCtrl = 0x00002808,
  VmcsField_kGuestIa32PerfGlobalCtrlHigh = 0x00002809,
  VmcsField_kGuestPdptr0 = 0x0000280a,
  VmcsField_kGuestPdptr0High = 0x0000280b,
  VmcsField_kGuestPdptr1 = 0x0000280c,
  VmcsField_kGuestPdptr1High = 0x0000280d,
  VmcsField_kGuestPdptr2 = 0x0000280e,
  VmcsField_kGuestPdptr2High = 0x0000280f,
  VmcsField_kGuestPdptr3 = 0x00002810,
  VmcsField_kGuestPdptr3High = 0x00002811,
  VmcsField_kGuestIa32Bndcfgs = 0x00002812,
  VmcsField_kGuestIa32BndcfgsHigh = 0x00002813,
  // 64-Bit Host-State Fields
  VmcsField_kHostIa32Pat = 0x00002c00,
  VmcsField_kHostIa32PatHigh = 0x00002c01,
  VmcsField_kHostIa32Efer = 0x00002c02,
  VmcsField_kHostIa32EferHigh = 0x00002c03,
  VmcsField_kHostIa32PerfGlobalCtrl = 0x00002c04,
  VmcsField_kHostIa32PerfGlobalCtrlHigh = 0x00002c05,
  // 32-Bit Control Fields
  VmcsField_kPinBasedVmExecControl = 0x00004000,
  VmcsField_kCpuBasedVmExecControl = 0x00004002,
  VmcsField_kExceptionBitmap = 0x00004004,
  VmcsField_kPageFaultErrorCodeMask = 0x00004006,
  VmcsField_kPageFaultErrorCodeMatch = 0x00004008,
  VmcsField_kCr3TargetCount = 0x0000400a,
  VmcsField_kVmExitControls = 0x0000400c,
  VmcsField_kVmExitMsrStoreCount = 0x0000400e,
  VmcsField_kVmExitMsrLoadCount = 0x00004010,
  VmcsField_kVmEntryControls = 0x00004012,
  VmcsField_kVmEntryMsrLoadCount = 0x00004014,
  VmcsField_kVmEntryIntrInfoField = 0x00004016,
  VmcsField_kVmEntryExceptionErrorCode = 0x00004018,
  VmcsField_kVmEntryInstructionLen = 0x0000401a,
  VmcsField_kTprThreshold = 0x0000401c,
  VmcsField_kSecondaryVmExecControl = 0x0000401e,
  VmcsField_kPleGap = 0x00004020,
  VmcsField_kPleWindow = 0x00004022,
  // 32-Bit Read-Only Data Fields
  VmcsField_kVmInstructionError = 0x00004400,  // See: VM-Instruction Error Numbers
  VmcsField_kVmExitReason = 0x00004402,
  VmcsField_kVmExitIntrInfo = 0x00004404,
  VmcsField_kVmExitIntrErrorCode = 0x00004406,
  VmcsField_kIdtVectoringInfoField = 0x00004408,
  VmcsField_kIdtVectoringErrorCode = 0x0000440a,
  VmcsField_kVmExitInstructionLen = 0x0000440c,
  VmcsField_kVmxInstructionInfo = 0x0000440e,
  // 32-Bit Guest-State Fields
  VmcsField_kGuestEsLimit = 0x00004800,
  VmcsField_kGuestCsLimit = 0x00004802,
  VmcsField_kGuestSsLimit = 0x00004804,
  VmcsField_kGuestDsLimit = 0x00004806,
  VmcsField_kGuestFsLimit = 0x00004808,
  VmcsField_kGuestGsLimit = 0x0000480a,
  VmcsField_kGuestLdtrLimit = 0x0000480c,
  VmcsField_kGuestTrLimit = 0x0000480e,
  VmcsField_kGuestGdtrLimit = 0x00004810,
  VmcsField_kGuestIdtrLimit = 0x00004812,
  VmcsField_kGuestEsArBytes = 0x00004814,
  VmcsField_kGuestCsArBytes = 0x00004816,
  VmcsField_kGuestSsArBytes = 0x00004818,
  VmcsField_kGuestDsArBytes = 0x0000481a,
  VmcsField_kGuestFsArBytes = 0x0000481c,
  VmcsField_kGuestGsArBytes = 0x0000481e,
  VmcsField_kGuestLdtrArBytes = 0x00004820,
  VmcsField_kGuestTrArBytes = 0x00004822,
  VmcsField_kGuestInterruptibilityInfo = 0x00004824,
  VmcsField_kGuestActivityState = 0x00004826,
  VmcsField_kGuestSmbase = 0x00004828,
  VmcsField_kGuestSysenterCs = 0x0000482a,
  VmcsField_kVmxPreemptionTimerValue = 0x0000482e,
  // 32-Bit Host-State Field
  VmcsField_kHostIa32SysenterCs = 0x00004c00,
  // Natural-Width Control Fields
  VmcsField_kCr0GuestHostMask = 0x00006000,
  VmcsField_kCr4GuestHostMask = 0x00006002,
  VmcsField_kCr0ReadShadow = 0x00006004,
  VmcsField_kCr4ReadShadow = 0x00006006,
  VmcsField_kCr3TargetValue0 = 0x00006008,
  VmcsField_kCr3TargetValue1 = 0x0000600a,
  VmcsField_kCr3TargetValue2 = 0x0000600c,
  VmcsField_kCr3TargetValue3 = 0x0000600e,
  // Natural-Width Read-Only Data Fields
  VmcsField_kExitQualification = 0x00006400,
  VmcsField_kIoRcx = 0x00006402,
  VmcsField_kIoRsi = 0x00006404,
  VmcsField_kIoRdi = 0x00006406,
  VmcsField_kIoRip = 0x00006408,
  VmcsField_kGuestLinearAddress = 0x0000640a,
  // Natural-Width Guest-State Fields
  VmcsField_kGuestCr0 = 0x00006800,
  VmcsField_kGuestCr3 = 0x00006802,
  VmcsField_kGuestCr4 = 0x00006804,
  VmcsField_kGuestEsBase = 0x00006806,
  VmcsField_kGuestCsBase = 0x00006808,
  VmcsField_kGuestSsBase = 0x0000680a,
  VmcsField_kGuestDsBase = 0x0000680c,
  VmcsField_kGuestFsBase = 0x0000680e,
  VmcsField_kGuestGsBase = 0x00006810,
  VmcsField_kGuestLdtrBase = 0x00006812,
  VmcsField_kGuestTrBase = 0x00006814,
  VmcsField_kGuestGdtrBase = 0x00006816,
  VmcsField_kGuestIdtrBase = 0x00006818,
  VmcsField_kGuestDr7 = 0x0000681a,
  VmcsField_kGuestRsp = 0x0000681c,
  VmcsField_kGuestRip = 0x0000681e,
  VmcsField_kGuestRflags = 0x00006820,
  VmcsField_kGuestPendingDbgExceptions = 0x00006822,
  VmcsField_kGuestSysenterEsp = 0x00006824,
  VmcsField_kGuestSysenterEip = 0x00006826,
  // Natural-Width Host-State Fields
  VmcsField_kHostCr0 = 0x00006c00,
  VmcsField_kHostCr3 = 0x00006c02,
  VmcsField_kHostCr4 = 0x00006c04,
  VmcsField_kHostFsBase = 0x00006c06,
  VmcsField_kHostGsBase = 0x00006c08,
  VmcsField_kHostTrBase = 0x00006c0a,
  VmcsField_kHostGdtrBase = 0x00006c0c,
  VmcsField_kHostIdtrBase = 0x00006c0e,
  VmcsField_kHostIa32SysenterEsp = 0x00006c10,
  VmcsField_kHostIa32SysenterEip = 0x00006c12,
  VmcsField_kHostRsp = 0x00006c14,
  VmcsField_kHostRip = 0x00006c16
};

enum VmxExitReason{
  VmxExitReason_kExceptionOrNmi = 0,
  VmxExitReason_kExternalInterrupt = 1,
  VmxExitReason_kTripleFault = 2,
  VmxExitReason_kInit = 3,
  VmxExitReason_kSipi = 4,
  VmxExitReason_kIoSmi = 5,
  VmxExitReason_kOtherSmi = 6,
  VmxExitReason_kPendingInterrupt = 7,
  VmxExitReason_kNmiWindow = 8,
  VmxExitReason_kTaskSwitch = 9,
  VmxExitReason_kCpuid = 10,
  VmxExitReason_kGetSec = 11,
  VmxExitReason_kHlt = 12,
  VmxExitReason_kInvd = 13,
  VmxExitReason_kInvlpg = 14,
  VmxExitReason_kRdpmc = 15,
  VmxExitReason_kRdtsc = 16,
  VmxExitReason_kRsm = 17,
  VmxExitReason_kVmcall = 18,
  VmxExitReason_kVmclear = 19,
  VmxExitReason_kVmlaunch = 20,
  VmxExitReason_kVmptrld = 21,
  VmxExitReason_kVmptrst = 22,
  VmxExitReason_kVmread = 23,
  VmxExitReason_kVmresume = 24,
  VmxExitReason_kVmwrite = 25,
  VmxExitReason_kVmoff = 26,
  VmxExitReason_kVmon = 27,
  VmxExitReason_kCrAccess = 28,
  VmxExitReason_kDrAccess = 29,
  VmxExitReason_kIoInstruction = 30,
  VmxExitReason_kMsrRead = 31,
  VmxExitReason_kMsrWrite = 32,
  VmxExitReason_kInvalidGuestState = 33,  // See: BASIC VM-ENTRY CHECKS
  VmxExitReason_kMsrLoading = 34,
  VmxExitReason_kUndefined35 = 35,
  VmxExitReason_kMwaitInstruction = 36,
  VmxExitReason_kMonitorTrapFlag = 37,
  VmxExitReason_kUndefined38 = 38,
  VmxExitReason_kMonitorInstruction = 39,
  VmxExitReason_kPauseInstruction = 40,
  VmxExitReason_kMachineCheck = 41,
  VmxExitReason_kUndefined42 = 42,
  VmxExitReason_kTprBelowThreshold = 43,
  VmxExitReason_kApicAccess = 44,
  VmxExitReason_kVirtualizedEoi = 45,
  VmxExitReason_kGdtrOrIdtrAccess = 46,
  VmxExitReason_kLdtrOrTrAccess = 47,
  VmxExitReason_kEptViolation = 48,
  VmxExitReason_kEptMisconfig = 49,
  VmxExitReason_kInvept = 50,
  VmxExitReason_kRdtscp = 51,
  VmxExitReason_kVmxPreemptionTime = 52,
  VmxExitReason_kInvvpid = 53,
  VmxExitReason_kWbinvd = 54,
  VmxExitReason_kXsetbv = 55,
  VmxExitReason_kApicWrite = 56,
  VmxExitReason_kRdrand = 57,
  VmxExitReason_kInvpcid = 58,
  VmxExitReason_kVmfunc = 59,
  VmxExitReason_kUndefined60 = 60,
  VmxExitReason_kRdseed = 61,
  VmxExitReason_kUndefined62 = 62,
  VmxExitReason_kXsaves = 63,
  VmxExitReason_kXrstors = 64,
};

// MSR Msr_kIa32VmxProcBasedCtls/Msr_kIa32VmxTrueProcBasedCtls  0x482/0x48E
typedef union _Vmx_ProcessorBased_Controls {
  uint32_t all;
  struct {
    uint32_t reserved1 : 2;                   //!< [0:1]
    uint32_t interrupt_window_exiting : 1;    //!< [2]
    uint32_t use_tsc_offseting : 1;           //!< [3]
    uint32_t reserved2 : 3;                   //!< [4:6]
    uint32_t hlt_exiting : 1;                 //!< [7]
    uint32_t reserved3 : 1;                   //!< [8]
    uint32_t invlpg_exiting : 1;              //!< [9]
    uint32_t mwait_exiting : 1;               //!< [10]
    uint32_t rdpmc_exiting : 1;               //!< [11]
    uint32_t rdtsc_exiting : 1;               //!< [12]
    uint32_t reserved4 : 2;                   //!< [13:14]
    uint32_t cr3_load_exiting : 1;            //!< [15]
    uint32_t cr3_store_exiting : 1;           //!< [16]
    uint32_t reserved5 : 2;                   //!< [17:18]
    uint32_t cr8_load_exiting : 1;            //!< [19]
    uint32_t cr8_store_exiting : 1;           //!< [20]
    uint32_t use_tpr_shadow : 1;              //!< [21]
    uint32_t nmi_window_exiting : 1;          //!< [22]
    uint32_t mov_dr_exiting : 1;              //!< [23]
    uint32_t unconditional_io_exiting : 1;    //!< [24]
    uint32_t use_io_bitmaps : 1;              //!< [25]
    uint32_t reserved6 : 1;                   //!< [26]
    uint32_t monitor_trap_flag : 1;           //!< [27]
    uint32_t use_msr_bitmaps : 1;             //!< [28]
    uint32_t monitor_exiting : 1;             //!< [29]
    uint32_t pause_exiting : 1;               //!< [30]
    uint32_t activate_secondary_control : 1;  //!< [31]
  } fields;
}__attribute__((packed)) Vmx_ProcessorBased_Controls, *PVmx_ProcessorBased_Controls;

/// MSR Msr_kIa32VmxProcBasedCtls2 0x48B
typedef union _Vmx_SecondaryProcessorBased_Controls {
  uint32_t all;
  struct {
    uint32_t virtualize_apic_accesses : 1;            //!< [0]
    uint32_t enable_ept : 1;                          //!< [1]
    uint32_t descriptor_table_exiting : 1;            //!< [2]
    uint32_t enable_rdtscp : 1;                       //!< [3]
    uint32_t virtualize_x2apic_mode : 1;              //!< [4]
    uint32_t enable_vpid : 1;                         //!< [5]
    uint32_t wbinvd_exiting : 1;                      //!< [6]
    uint32_t unrestricted_guest : 1;                  //!< [7]
    uint32_t apic_register_virtualization : 1;        //!< [8]
    uint32_t virtual_interrupt_delivery : 1;          //!< [9]
    uint32_t pause_loop_exiting : 1;                  //!< [10]
    uint32_t rdrand_exiting : 1;                      //!< [11]
    uint32_t enable_invpcid : 1;                      //!< [12]
    uint32_t enable_vm_functions : 1;                 //!< [13]
    uint32_t vmcs_shadowing : 1;                      //!< [14]
    uint32_t reserved1 : 1;                           //!< [15]
    uint32_t rdseed_exiting : 1;                      //!< [16]
    uint32_t reserved2 : 1;                           //!< [17]
    uint32_t ept_violation_ve : 1;                    //!< [18]
    uint32_t reserved3 : 1;                           //!< [19]
    uint32_t enable_xsaves_xstors : 1;                //!< [20]
    uint32_t reserved4 : 1;                           //!< [21]
    uint32_t mode_based_execute_control_for_ept : 1;  //!< [22]
    uint32_t reserved5 : 2;                           //!< [23:24]
    uint32_t use_tsc_scaling : 1;                     //!< [25]
  } fields;
}__attribute__((packed)) Vmx_SecondaryProcessorBased_Controls, *PVmx_SecondaryProcessorBased_Controls;
