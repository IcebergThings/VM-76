//==============================================================================
// ■ ASM76的头文件
//==============================================================================

#include "stdlib"
#include "cstdio"
#include "stdint"

namespace ASM76 {

	enum InstructionSets {
		// 76-Base
		LCMM,
		LDLA, LDIA, LDBA,
		LDLR, LDIR, LDBR,
		SLAA, SLIA, SLBA,
		SLLR, SLIR, SLBR,
		ADDL, ADDI, ADDB,
		MINL, MINI, MINB,
		MTPL, MTPI, MTPB,
		DIVL, DIVI, DIVB,
		MODL, MODI, MODB,

		// Logistics & Flow control
		ANDL, ANDI, ANDB,
		OR_L, OR_I, OR_B,
		NOTL, NOTI, NOTB,
		XORL, XORI, XORB,
		CMPL, CMPI, CMPB,
		JMPR,
		JMPA,
		JI9R, JI8R, JI7R,
		CALR, CALA,
		PUSH,
		POP,

		// 76 Float

		// 76 Vectors

		// BIOS Instructions
	}

	extern char* global_memory;

	void init_environment();

	struct Instruct {
		uint16_t opcode;
		uint32_t f;
		uint32_t t;
	};


	class VM {
	private:
		// These two bufs are mirror to the same memory buffer
		char* local_memory();
		Instruct* instruct_memory();

	public:
		VM(Instruct* program);

		void execute();
		void dump_registers();
		void dump_memory();

		static char* decompile(Instruct* prg);
		static Instruct* compile(const char* prg);

		~VM();
	};

}
