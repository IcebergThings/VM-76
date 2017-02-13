//=============================================================================
// ■ instructions.hpp
//-----------------------------------------------------------------------------
//   独立的指令列表。通常在包含时会对宏I(x)进行处理。
//=============================================================================

I(NOOP) // No Operation is always NULL

// 76-Base
I(LCMM)
I(LDLA) I(LDIA) I(LDBA)
I(LDLR) I(LDIR) I(LDBR)
I(SLLA) I(SLIA) I(SLBA)
I(SLLR) I(SLIR) I(SLBR)
I(DATL) I(DATI) I(DATB)
I(MOVL) I(MOVI) I(MOVB)
I(MVPL) I(MVPI) I(MVPB)
I(MVRL) I(MVRI) I(MVRB)
I(ADDL) I(ADDI) I(ADDB)
I(MINL) I(MINI) I(MINB)
I(MTPL) I(MTPI) I(MTPB)
I(DIVL) I(DIVI) I(DIVB)
I(MODL) I(MODI) I(MODB)
I(_HLT)

// Logistics & Flow control
I(ANDL) I(ANDI) I(ANDB)
I(OR_L) I(OR_I) I(OR_B)
I(NOTL) I(NOTI) I(NOTB)
I(XORL) I(XORI) I(XORB)
I(CMPL) I(CMPI) I(CMPB)
I(JMPR)
I(JMPA)
I(JI9A) I(JI8A) I(JI7A)
I(JI9R) I(JI8R) I(JI7R)
I(CALR) I(CALA)
I(RET)
I(PUSH)
I(POP)

// 76 Float

// 76 Vectors

// BIOS Instructions
