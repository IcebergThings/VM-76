ASM76
=====

ASM76 is the assembly language used in VM/76. This document describes both the VM/76 virtual machine platform and the assembly language itself.

Definitions and conventions in this specification
-------------------------------------------------

**Byte** is defined as usual.

An **int** consists of 4 bytes.

A **long** consists of 8 bytes.

**0x** leads a hexadecimal number, e.g. 0x1F means 31.

**_x_.._y_** means a range that is inclusive at both ends, i.e. [*x*, *y*].

**_x_..._y_** means a range that includes *x* but excludes *y*, i.e. [*x*, *y*).

76 Virtual Machine and VM/76 CPU
--------------------------------

### One line for the virtual machine ###

The virtual machine compiles and runs assembly code.

### Introduction, the memory and registers ###

The virtual CPU (VM/76 CPU) is a little-endian CPU. It is capable of doing 64-bit calculations for integers by default, which can be extended by using instruction set extensions that allows floating point value and vector mathematics. The length of instruction sets are fixed. However, the CPU uses a 32-bit memory address space. The memory layout is fixed, too, and is specified below.

Address range | Size | Usage
------------- | ---- | -----
0x0...0x400000 | 4MB | This part of memory is global memory, which is shared between CPU instances.
0x400000...0x1000000 | 12MB | This part of memory is mapped with IO to transfer data between the CPU (ASM76) and the outside world (VMDE).
0x1000000...∞ | depends | The upper part of the memory is local, which can be sized using [LCMM] and belongs to the CPU instance.

As you can see, there can be multiple CPUs. One CPU is usually assigned exclusively to one thread, one sprite or all other kinds of things like that.

Another interesting concept about the CPU is registers. Normal computer CPUs have a very limited number of registers because they are required to perform as fast as possible. However, VM/76 CPU daren't be fast and thus have exactly one hundred registers available. All registers in VM/76 CPU 

There are a number of special registers that have special meanings and they are listed in [Special registers].

### Instruction sets ###



The ASM76 language syntax
-------------------------

Here is an example program written in ASM76:

```
# ASM76 Example
# Hash signs start comments.

```

A few notes:

- The instruction mnemonic must be capital.
- 

Reference material
------------------

### Memory layout ###

Address range | Size | Usage
------------- | ---- | -----
0x0...0x400000 | 4MB | This part of memory is global memory, which is shared between CPU instances.
0x400000...0x1000000 | 12MB | This part of memory is mapped with IO to transfer data between the CPU (ASM76) and the outside world (VMDE).
0x1000000...∞ | depends | The upper part of the memory is local, which can be sized using [LCMM] and belongs to the CPU instance.

### Special registers ###

Register | Meaning
-------- | -------
$99 | Flag A > B
$98 | Flag A = B
$97 | Flag A < B
$90 ~ $93 | Stack bottom pointer
$86 ~ $89 | Instruction pointer (program counter)


Instruction set reference
-------------------------

### 76-Base

76-Base is the most essential instruction set. It is enabled by default, You don't need to enable it and you can not turn it off in any way.

#### DATx















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
7. Mem operation
	-	`MOVL [adress A], [adress B]`
		- copy 8 bytes data from `adress A` to `adress B`
	-	`MVPL [$A], [$B]`
		- copy 8 bytes data from adress in `$A` to the address in `$B`
	-	`MVRL $A, $B`
		- copy 8 bytes data from `$A` to `$B`
	- `MOVI`, `MOVB`, `MVPI`, `MVPB` `MVRI`, `MVRB` are in similar pattern
8. Basic algebra
	-	`ADDL $A, $B`
		-	Do 8 bytes (64 bits) adding. When it's done, `$A = $A + $B`
	-	`MINL $A, $B`
		-	Do 8 bytes (64 bits) minus. When it's done, `$A = $A - $B`
	-	`MTPL $A, $B`
		- Do 8 bytes (64 bits) multiplying. `$A = $A * $B`
	-	`DIVL $A, $B`
		- Do 8 bytes (64 bits) integer dividing. `$A = $A / $B`
	- `MODL $A, $B`
		- `$A` equals the remainder of `$A / $B`
	-	`ADDI`, `ADDB`, `MINI`, `MINB`, `MTPI`, `MTPB`, `DIVI`, `DIVB`, `MODI`, `MODB` are similar to previous pattern.
7. `_HLT`
	-	Halt the VM and stop.

## Logistics & Flow control
1. Logical algebra
	-	`ANDL $A, $B` : `$A = $A & $B`
	-	`OR_L $A, $B` : `$A = $A | $B`
	-	`NOTL $A`     : `$A = !$A`
	-	`XORL $A, $B` : `$A = $A xor $B`
	- `CMPL $A, $B`
		-	Compare `$A` to `$B`
		-	If `$A > $B`, `$99 = 0xFF`, `$98, $97 = 0x0`
		-	If `$A == $B`, `$98 = 0xFF`, `$97, $99 = 0x0`
		-	If `$A < $B`, `$97 = 0xFF`, `$98, $99 = 0x0`
	- Similar instructions with Int and Byte are present.
2. Flow control
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
