//=============================================================================
// ■ instructions.hpp
//-----------------------------------------------------------------------------
//   指令注册表。必须在包含时定义宏I(x, ta, tb)。本文件则负责清理这个宏。
//=============================================================================

// I(opcode, operand 1 type, operand 2 type) - Instruction

I(NOOP, TNUL, TNUL) // No Operation is always NULL

// 76-Base
I(ADDL, TREG, TREG) I(ADDI, TREG, TREG) I(ADDB, TREG, TREG)
I(MINL, TREG, TREG) I(MINI, TREG, TREG) I(MINB, TREG, TREG)
I(MTPL, TREG, TREG) I(MTPI, TREG, TREG) I(MTPB, TREG, TREG)
I(DIVL, TREG, TREG) I(DIVI, TREG, TREG) I(DIVB, TREG, TREG)
I(MODL, TREG, TREG) I(MODI, TREG, TREG) I(MODB, TREG, TREG)
I(MOVL, TADD, TADD) I(MOVI, TADD, TADD) I(MOVB, TADD, TADD)
I(LDLA, TADD, TREG) I(LDIA, TADD, TREG) I(LDBA, TADD, TREG)
I(LDLR, TREG, TREG) I(LDIR, TREG, TREG) I(LDBR, TREG, TREG)
I(SLLA, TADD, TREG) I(SLIA, TADD, TREG) I(SLBA, TADD, TREG)
I(SLLR, TREG, TREG) I(SLIR, TREG, TREG) I(SLBR, TREG, TREG)
I(MVRL, TREG, TREG) I(MVRI, TREG, TREG) I(MVRB, TREG, TREG)
I(MVPL, TREG, TREG) I(MVPI, TREG, TREG) I(MVPB, TREG, TREG)
I(DATL, TIMM, TREG) I(DATI, TIMM, TREG) I(DATB, TIMM, TREG)
I(HALT, TNUL, TNUL)
I(LCMM, TIMM, TNUL)

// Logistics & Flow control
I(ANDL, TREG, TREG) I(ANDI, TREG, TREG) I(ANDB, TREG, TREG)
I(OR_L, TREG, TREG) I(OR_I, TREG, TREG) I(OR_B, TREG, TREG)
I(NOTL, TREG, TREG) I(NOTI, TREG, TREG) I(NOTB, TREG, TREG)
I(XORL, TREG, TREG) I(XORI, TREG, TREG) I(XORB, TREG, TREG)
I(CMPL, TREG, TREG) I(CMPI, TREG, TREG) I(CMPB, TREG, TREG)
I(JMPR, TREG, TNUL) I(JMPA, TADD, TNUL)
I(JIGA, TADD, TNUL) I(JIEA, TADD, TNUL) I(JILA, TADD, TNUL)
I(JIGR, TREG, TNUL) I(JIER, TREG, TNUL) I(JILR, TREG, TNUL)
I(CALR, TREG, TNUL) I(CALA, TADD, TNUL)
I(RETN, TNUL, TNUL)
I(PUSH, TREG, TIMM) I(POP_, TREG, TIMM)

// 76 Float

// 76 Vectors

// BIOS Instructions
I(INTX, TIMM, TADD)
I(INTR, TREG, TADD)

// 自带清理宏
#undef I
#ifdef TNUL
	// in case of some really annoying preprocessors
	#undef TNUL
	#undef TIMM
	#undef TADD
	#undef TREG
#endif
// 这个REG宏名字重复了，又被坑了一回。
