#ifndef MIPS_HEADER
#define MIPS_HEADER

/*****************************************************************
*                                                                *
*    MIPS Defines: Numerical parameters of the MIPS processor    *
*                                                                *
*****************************************************************/

/*******************
* USEFUL CONSTANTS *
*******************/

// Mask for the coprocessor number from the opcode
#define OP__ZMASK       0x03

// Mask particular bytes
#define BYTE_0_MASK     0x000000ff
#define BYTE_1_MASK     0x0000ff00
#define BYTE_2_MASK     0x00ff0000
#define BYTE_3_MASK     0xff000000
#define BYTE_0_1_MASK   0x0000ffff
#define BYTE_0_2_MASK   0x00ff00ff
#define BYTE_0_3_MASK   0xff0000ff
#define BYTE_1_2_MASK   0x00ffff00
#define BYTE_1_3_MASK   0xff00ff00
#define BYTE_2_3_MASK   0xffff0000
#define BYTE_0_1_2_MASK 0x00ffffff
#define BYTE_0_1_3_MASK 0xff00ffff
#define BYTE_0_2_3_MASK 0xffff00ff
#define BYTE_1_2_3_MASK 0xffffff00


/**********
* OPCODES *
**********/

// Main opcodes (op field)
#define OP_OTHER0   0x00
#define OP_OTHER1   0x01
#define OP_J        0x02
#define OP_JAL      0x03
#define OP_BEQ      0x04
#define OP_BNE      0x05
#define OP_BLEZ     0x06
#define OP_BGTZ     0x07
#define OP_ADDI     0x08
#define OP_ADDIU    0x09
#define OP_SLTI     0x0a
#define OP_SLTIU    0x0b
#define OP_ANDI     0x0c
#define OP_ORI      0x0d
#define OP_XORI     0x0e
#define OP_LUI      0x0f
#define OP_Z0       0x10
#define OP_Z1       0x11
#define OP_Z2       0x12
#define OP_Z3       0x13
#define OP_SPECIAL2 0x1c
#define OP_JALX     0x1d
#define OP_SPECIAL3 0x1f
#define OP_LB       0x20
#define OP_LH       0x21
#define OP_LWL      0x22
#define OP_LW       0x23
#define OP_LBU      0x24
#define OP_LHU      0x25
#define OP_LWR      0x26
#define OP_SB       0x28
#define OP_SH       0x29
#define OP_SWL      0x2a
#define OP_SW       0x2b
#define OP_SWR      0x2e
#define OP_CACHE    0x2f
#define OP_LL       0x30
#define OP_LWC1     0x31
#define OP_LWC2     0x32
#define OP_PREF     0x33
#define OP_LDC1     0x35
#define OP_LDC2     0x36
#define OP_SC       0x38
#define OP_SWC1     0x39
#define OP_SWC2     0x3a
#define OP_SDC1     0x3d
#define OP_SDC2     0x3e

// Secondary opcodes (rs field; OP_Z[0-3])
#define OPZ_MFCZ    0x00
#define OPZ_CFCZ    0x02
#define OPZ_MTCZ    0x04
#define OPZ_CTCZ    0x06
#define OPZ_BCZ     0x08
#define OPZ_COPZS   0x10
#define OPZ_COPZD   0x11

// Tertiary opcodes (bczft field; OP_Z[0-3] && OPZ_BCZ)
#define OPB_BCZF    0x0
#define OPB_BCZT    0x1

// Tertiary opcodes (funct1 field; OP_Z0 && OPZ_COPZS)
#define OPC_TLBR    0x01
#define OPC_TLBWI   0x02
#define OPC_TLBWR   0x06
#define OPC_TLBP    0x08
#define OPC_RFE     0x10

// Secondary opcodes (rt field; OP_OTHER1)
#define OP1_BLTZ    0x00
#define OP1_BGEZ    0x01
#define OP1_BLTZAL  0x10
#define OP1_BGEZAL  0x11

// Secondary opcodes (funct2 field; OP_OTHER0)
#define OP0_SLL     0x00
#define OP0_SRL     0x02
#define OP0_SRA     0x03
#define OP0_SLLV    0x04
#define OP0_SRLV    0x06
#define OP0_SRAV    0x07
#define OP0_JR      0x08
#define OP0_JALR    0x09
#define OP0_SYSCALL 0x0c
#define OP0_BREAK   0x0d
#define OP0_MFHI    0x10
#define OP0_MTHI    0x11
#define OP0_MFLO    0x12
#define OP0_MTLO    0x13
#define OP0_MULT    0x18
#define OP0_MULTU   0x19
#define OP0_DIV     0x1a
#define OP0_DIVU    0x1b
#define OP0_ADD     0x20
#define OP0_ADDU    0x21
#define OP0_SUB     0x22
#define OP0_SUBU    0x23
#define OP0_AND     0x24
#define OP0_OR      0x25
#define OP0_XOR     0x26
#define OP0_NOR     0x27
#define OP0_SLT     0x2a
#define OP0_SLTU    0x2b

// Tertiary opcodes (funct2 field; OP_Z1 && (OPZ_COPZS || OPZ_COPZD))
#define OPF_ADDF    0x00
#define OPF_SUBF    0x01
#define OPF_MULF    0x02
#define OPF_DIVF    0x03
#define OPF_ABSF    0x05
#define OPF_MOVF    0x06
#define OPF_NEGF    0x07
#define OPF_CVTSF   0x20
#define OPF_CVTDF   0x21
#define OPF_CVTWF   0x24
#define OPF_CFF     0x30
#define OPF_CUNF    0x31
#define OPF_CEQF    0x32
#define OPF_CUEQF   0x33
#define OPF_COLTF   0x34
#define OPF_CULTF   0x35
#define OPF_COLEF   0x36
#define OPF_CULEF   0x37
#define OPF_CSFF    0x38
#define OPF_CNGLEF  0x39
#define OPF_CSEQF   0x3a
#define OPF_CNGLF   0x3b
#define OPF_CLTF    0x3c
#define OPF_CNGEF   0x3d
#define OPF_CLEF    0x3e
#define OPF_CNGTF   0x3f

// Coprocessor 0 register mnemonics
#define CR_EPC      0x14
#define CR_BADVADDR 0x8
#define CR_STATUS   0x12
#define CR_CAUSE    0x13

// Floating point registers (coprocessor 1)
#define R_F0        0
#define R_F1        1
#define R_F2        2
#define R_F3        3
#define R_F4        4
#define R_F5        5
#define R_F6        6
#define R_F7        7
#define R_F8        8
#define R_F9        9
#define R_F10       10
#define R_F11       11
#define R_F12       12
#define R_F13       13
#define R_F14       14
#define R_F15       15
#define R_F16       16
#define R_F17       17
#define R_F18       18
#define R_F19       19
#define R_F20       20
#define R_F21       21
#define R_F22       22
#define R_F23       23
#define R_F24       24
#define R_F25       25
#define R_F26       26
#define R_F27       27
#define R_F28       28
#define R_F29       29
#define R_F30       30
#define R_F31       31

/******************
* Exception Codes *
******************/

// Left shift amount to align exception code from cause register
#define EX__SHIFT 0x2
// Mask to extract exception code from CAUSE register
#define EX__MASK  0x0000007c

// Exception codes
#define EX_INT    0x0
#define EX_MOD    0x1
#define EX_TLBL   0x2
#define EX_TLBS   0x3
#define EX_ADEL   0x4
#define EX_ADES   0x5
#define EX_IBE    0x6
#define EX_DBE    0x7
#define EX_SYS    0x8
#define EX_BP     0x9
#define EX_RI     0x10
#define EX_CPU    0x11
#define EX_OV     0x12
#define EX_TR     0x13
#define EX_VCEI   0x14
#define EX_FPE    0x15
#define EX_WATCH  0x23
#define EX_VCED   0x31

#endif
