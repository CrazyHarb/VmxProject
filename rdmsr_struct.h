enum Msr{
	Msr_kIa32ApicBase = 0x01B,

	Msr_kIa32FeatureControl = 0x03A,

	Msr_kIa32SysenterCs = 0x174,
	Msr_kIa32SysenterEsp = 0x175,
	Msr_kIa32SysenterEip = 0x176,

	Msr_kIa32Debugctl = 0x1D9,

	Msr_kIa32MtrrCap = 0xFE,
	Msr_kIa32MtrrDefType = 0x2FF,
	Msr_kIa32MtrrPhysBaseN = 0x200,
	Msr_kIa32MtrrPhysMaskN = 0x201,
	Msr_kIa32MtrrFix64k00000 = 0x250,
	Msr_kIa32MtrrFix16k80000 = 0x258,
	Msr_kIa32MtrrFix16kA0000 = 0x259,
	Msr_kIa32MtrrFix4kC0000 = 0x268,
	Msr_kIa32MtrrFix4kC8000 = 0x269,
	Msr_kIa32MtrrFix4kD0000 = 0x26A,
	Msr_kIa32MtrrFix4kD8000 = 0x26B,
	Msr_kIa32MtrrFix4kE0000 = 0x26C,
	Msr_kIa32MtrrFix4kE8000 = 0x26D,
	Msr_kIa32MtrrFix4kF0000 = 0x26E,
	Msr_kIa32MtrrFix4kF8000 = 0x26F,

	Msr_kIa32VmxBasic = 0x480,
	Msr_kIa32VmxPinbasedCtls = 0x481,
	Msr_kIa32VmxProcBasedCtls = 0x482,
	Msr_kIa32VmxExitCtls = 0x483,
	Msr_kIa32VmxEntryCtls = 0x484,
	Msr_kIa32VmxMisc = 0x485,
	Msr_kIa32VmxCr0Fixed0 = 0x486,
	Msr_kIa32VmxCr0Fixed1 = 0x487,
	Msr_kIa32VmxCr4Fixed0 = 0x488,
	Msr_kIa32VmxCr4Fixed1 = 0x489,
	Msr_kIa32VmxVmcsEnum = 0x48A,
	Msr_kIa32VmxProcBasedCtls2 = 0x48B,
	Msr_kIa32VmxEptVpidCap = 0x48C,
	Msr_kIa32VmxTruePinbasedCtls = 0x48D,
	Msr_kIa32VmxTrueProcBasedCtls = 0x48E,
	Msr_kIa32VmxTrueExitCtls = 0x48F,
	Msr_kIa32VmxTrueEntryCtls = 0x490,
	Msr_kIa32VmxVmfunc = 0x491,

	Msr_kIa32Efer = 0xC0000080,
	Msr_kIa32Star = 0xC0000081,
	Msr_kIa32Lstar = 0xC0000082,

	Msr_kIa32Fmask = 0xC0000084,

	Msr_kIa32FsBase = 0xC0000100,
	Msr_kIa32GsBase = 0xC0000101,
	Msr_kIa32KernelGsBase = 0xC0000102,
	Msr_kIa32TscAux = 0xC0000103,
};