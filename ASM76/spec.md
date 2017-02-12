# ASM 76
The VM/76 assembly language and VM/76 virtual machine platform

---

# 76 Virtual Machine
1. Little Endian Virtual CPU
2. 64bit int/float/vector calculations
3. 32bit memory adress
4. Instruction set extension
5. Fixed length instruction sets
6. Memory Layout
	-	`0x0` ~ `0x400000` 4MB, mapped with global memory. This part of memory is the same for all VM instances.
	- `0x400000` ~ `0x1000000` 12MB, mapped with IO to transfer data between VM and Game Engine
	- `0x1000000` ~ `Infinite` Sized depends on LCMM instruction, mapped with local memory. This part of memory is not shared between multiple VM instance (such as threads or VM in multiple spirits)

---

# Instruction sets
## 76-Base
1. How to enable it: Enabled default, you can not turn it off
2. Registers
	- 99 registers available
	- Named as `$0 $1 $2`, etc.
	- Each register is 8 bit
	- When doing calculations more than 8 bits, it will start from the first register and connect several of them
		- `$0` -> 8bit default
		- `MOVL $0, $10` -> $(0 ~ 8) all moved to `$(10 ~ 18)`
	- **Special registers**
		-	`$99` Flag A > B
		-	`$98` Flag A == B
		- `$97` Flag A < B
		-	`$90-93` Stack bottom pointer
		-	`$86-89` Instruction pointer
3. `LCMM [size]`
	-	Specify local memory size (in bytes)
	-	It does not has a maximum limit in theory
	-	It is 16KB by default
	-	As the command runs, the data in the memory will be cleared and initialized with all zero. Then it will copy previous data into the memory. If the previous data is longer, it will be truncated.
4. Load data in memory
	-	`LDLA [Local Memory Adress], [Register]`
		- Load Local Long (8 bytes) from Adress
		- Load 64 bits from local memory address to a sequence of 8 registers. [Register] specified the first one of this 8 registers sequence.
	- `LDIA [Local Memory Adress], [Register]`
		- Load Local INT (4 bytes) from Adress
	- `LDBA [Local Memory Adress], [Register]`
		- Load Local BYTE (1 byte) from Adress
	- `LDLR [Register A], [Register B]`
		- Load Local Long (8 bytes) from adress stored in Register A to Register B
		- E.g.
			1. Store adress `0x00FF0000`to `$0`
			2. `LLLR $0, $12`, 8 bytes of data in `0x00FF0000` will be stored in `$12`
	- `LDIR`, `LDBR` are similar to previous specification.
5. Store data to memory
	- `SLLA [Local Memory Adress], [Register]`
		- Store 8 bytes from Register to local memory adress
	- `SLIA`, `SLBA` are in the similar parttern.
	-	`SLLR`, `SLIR`, `SLBR` are in the similar pattern with pointer in register
	- `DATL [value], $A`
		- Put long value in regster `$A`
	- Similar pattern with `DATI`, `DATB`
6. Basic algebra
	-	`ADDL $A, $B`
		-	Do 8 bytes (64 bits) adding. When it's done, `$B = $A + $B`
	-	`MINL $A, $B`
		-	Do 8 bytes (64 bits) minus. When it's done, `$B = $A - $B`
	-	`MTPL $A, $B`
		- Do 8 bytes (64 bits) multiplying. `$B = $A * $B`
	-	`DIVL $A, $B`
		- Do 8 bytes (64 bits) integer dividing. `$B = $A / $B`
	- `MODL $A, $B`
		- `$B` equals the remainder of `$A / $B`
	-	`ADDI`, `ADDB`, `MINI`, `MINB`, `MTPI`, `MTPB`, `DIVI`, `DIVB`, `MODI`, `MODB` are similar to previous pattern.
7. `_HLT`
	-	Halt the VM and stop.

## Logistics & Flow control
1. How to enable: Set memory `IO + 0x0` (`0x400000`, one byte) to `0xFF`
2. Logical algebra
	-	`ANDL $A, $B` : `$B = $A & $B`
	-	`OR_L $A, $B` : `$B = $A | $B`
	-	`NOTL $A`     : `$A = !$A`
	-	`XORL $A, $B` : `$A = $A xor $B`
	- `CMPL $A, $B`
		-	Compare `$A` to `$B`
		-	If `$A > $B`, `$99 = 0xFF`, `$98, $97 = 0x0`
		-	If `$A == $B`, `$98 = 0xFF`, `$97, $99 = 0x0`
		-	If `$A < $B`, `$97 = 0xFF`, `$98, $99 = 0x0`
	- Similar instructions with Int and Byte are present.
3. Flow control
	-	`JMPR $A`
		- jump to memory adress stored in `$A`
	-	`JMPA [Adress]`
		- jump to memory adress `[Adress]`
	-	`JI9R $A`, `JI9A [Adress]`
		- jump to memory adress stored in `$A` or `[Adress]` when `$99 == 0xFF`
	-	`JI8R $A`, `JI9A [Adress]`
		- jump to memory adress stored in `$A` or `[Adress]` when `$98 == 0xFF`
	-	`JI7R $A`, `JI9A [Adress]`
		- jump to memory adress stored in `$A` or `[Adress]` when `$97 == 0xFF`
	-	`$90` is the stack pointer.
		- It points to `0x1003000` by default, which is Local memory with offset 12KB.
	-	`CALR $A`, `CALA [Adress]`
		- Jump to memory adress stored in `$A` or `[Adress]`
		- Push the next instruction's adress into stack
	-	`PUSH $A, [Length]`
		- Push registers from `$A` to `$(A + Length)` into a stack
	-	`POP $A, [Length]`
		- Push data from stack to registers `$A` to `$(A + Length)`

## 76 Float
1. How to enable: Set memory `IO + 0x1` (`0x400000`, one byte) to `0xFF`

TODO

## 76 Vectors
1. How to enable: Set memory `IO + 0x2` (`0x400001`, one byte) to `0xFF`

TODO

## BIOS Instructions
1. How to enable: Set memory `IO + 0xA` (`0x40000A`, one byte) to `0xFF`

TODO
