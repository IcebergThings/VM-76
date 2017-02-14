ASM76 specification
===================

This document describes both the VM/76 virtual machine platform and the assembly language itself.

Definitions and conventions in this specification
-------------------------------------------------

- **Bit** is defined as usual.
- **Byte** consists of 8 bits.
- An **int** consists of 4 bytes.
- A **long** consists of 8 bytes.
- **0x** leads a hexadecimal number, e.g. 0x1F means 1F<sub>16</sub> (31<sub>10</sub>).
- **_x_.._y_** means a range that is inclusive at both ends, i.e. [*x*, *y*].
- **_x_..._y_** means a range that includes *x* but excludes *y*, i.e. [*x*, *y*).

76 Virtual Machine and VM/76 CPU
--------------------------------

- The virtual machine compiles and runs assembly code.
- **ASM76** is the assembly language used in VM/76.
	- It has fixed length instruction mnemonics.
- **VM/76 CPU** is the virtual CPU.
	- It is little-endian.
		- It can only be emulated on native little-endian machines at present.
		- Running the virtual machine on big-endian machines will result unspecifiedly.
	- There are usually multiple instances. One CPU is usually assigned exclusively to one thread, one sprite, etc.
	- Since it isn't used in real world, it is *quite different* from the CPUs from common ones, such as Intel x86 series CPUs.
- Instruction sets
	- **[76-Base](#76-base)** is capable of manipulating registers, memory and 8-, 32- and 64-bit integers.
	- **[76-Float](#76-float)** is able to deal with floating point values.
	- **[76-Vector](#76-vector)** can do vector mathematics.
	- **[BIOS Instructions](#bios-instructions)** provides a BIOS interface. Just joking.
- Registers
	- There are 112 registers in all, namely $0, $1, $2, …, $111.
	- Every register holds a byte.
	- The method of manipulation is the same among all registers.
	- Register $100 and above have special meanings.

		Register | Meaning | Default value
		-------- | ------- | -------------
		$100..$103 | Instruction Pointer | 0x01000000
		$104..$107 | Stack Pointer | 0x01003000
		$109 == 0x0 | Flag A < B | 0
		$109 == 0x1 | Flag A = B | 0
		$109 == 0x2 | Flag A > B | 0

- 32-bit memory address

	Address range | Size | Usage
	------------- | ---- | -----
	0x0...0x400000 | 4MB | This part of memory is global memory, which is shared between CPU instances.
	0x400000...0x1000000 | 12MB | This part of memory is mapped with IO to transfer data between the CPU (ASM76) and the outside world (VMDE).
	0x1000000...∞ | depends | The upper part of the memory is local, which can be sized using [LCMM](#lcmm) and belongs to the CPU instance.

The ASM76 language syntax
-------------------------

```asm
# ASM76 Example
# Hash signs start comments.
# Only whole line comments are allowed.

# Hexadecimal numbers.
LCMM 0x100

# Decimal numbers and registers.
# Parameters are separated by spaces, not commas.
DATI 1 $1
ADDL $1 $1
ADDL $1 $1
ADDL $1 $1
ADDL $1 $1

# Register No. is decimal.
MVRL $1 $31
DATI 4 $11
DIVL $1 $11
HALT
```

Instruction set reference
=========================

Conventions
-----------

- The instruction mnemonic must be capital.
- Here's a list of common postfixes in mnemonics.
	- ‘B’, ‘I’ and ‘L’ stands for byte, int and long, respectively.
		- For simplicity, they may be replaced by ‘x’ in this reference.
		- E.g. MOV**B** MOV**I** MOV**L**
	- ‘R’ means register and ‘A’ means address.
		- E.g. JMP**R** JMP**A**
	- ‘_’ (underline) is used as a blank filler in mnemonics.
		- E.g. OR**_**L POP**_**
- Since a register can only hold a byte, when doing calculations more than a byte, it will start from the specified register and use the following consecutive registers.
	- For example, `MOVL $0, $10` makes $0 ~ $8 all moved to $10 ~ $18.
- Italic font (*A*) denotes a parameter.
- Italic font prepended with a dollar sign (*$A*) denotes a register parameter.

76-Base
-------

76-Base is the most essential instruction set. It is enabled by default, You don't need to enable it and you can not turn it off in any way.

### Memory and registers

To simplify the description, we'll use some word macros.

- **$A** ⩴ the value in register $A
- **[A]** ⩴ the value in the memory address A
- **[$A]** ⩴ the value in the memory address specified by the value in register $A
- **A ← B** ⩴ put B into A

Instruction | Description
----------- | -----------
[LCMM](#lcmm) _size in bytes_ | set local memory size
DATx _data_, _$A_ | *$A* ← *data*
[LDxA](#ldxa) _A_, _$B_ | *$B* ← [*A*]
[LDxR](#ldxr) _$A_, _$B_ | *$B* ← [*$A*]
SLxA _A_, _$B_ | [*A*] ← *$B*
SLxR _$A_, _$B_ | [*$A*] ← *$B*
MOVx _A_, _B_ | [*B*] ← [*A*]
MVRx _$A_, _$B_ | *$B* ← *$A*
MVPx _$A_, _$B_ | [*$B*] ← [*$A*]

#### LCMM
Specifiy the local memory size. It does not has a maximum limit in theory and is 16KB by default. As the command runs, the data in the memory will be cleared and initialized with all zero. Then it will copy previous data into the memory. If the previous data is longer, it will be truncated.

#### LDxA
Load a long/int/byte from local memory address to a sequence of 8/4/1 register(s). *register* specifies the first one of the 8/4/1 register(s) sequence.

#### LDxR
For example:

```asm
DATI 0x00FF0000 $0
LDLR $0 $12
```

Then 8 bytes of data in 0x00FF0000 will be stored in $12.

### Basic algebra

Instruction | Description
----------- | -----------
ADDx _$A_, _$B_ | *$A* ← *$A* + *$B*
MINx _$A_, _$B_ | *$A* ← *$A* − *$B*
MTPx _$A_, _$B_ | *$A* ← *$A* × *$B*
[DIVx](#divxmodx) _$A_, _$B_ | *$A* ← ⌊*$A* ÷ *$B*⌋
[MODx](#divxmodx) _$A_, _$B_ | *$A* ← *$A* mod *$B*
[CMPx](#cmpx) _$A_, _$B_ | compare two long/int/byte arithmetically

#### DIVx/MODx
- The division throws the remainder away.
- The modulo throws the quotient away.

#### CMPx
Compare *$A* to *$B*.

- If *$A* > *$B*, $111 = 0xFF, $110 = 0, $109 = 0
- If *$A* = *$B*, $110 = 0xFF, $109 = 0, $111 = 0
- If *$A* < *$B*, $109 = 0xFF, $110 = 0, $111 = 0

### Logical operations

Instruction | Description
----------- | -----------
ANDx/OR_x/XORx _$A_, _$B_ | logical AND/OR/XOR for long/int/byte
[NOTx](#notx) _$A_ | boolean not for long/int/byte

#### NOTx
Means `$A = !$A;` in C.

- If [*$A*] = 0, then *$A* becomes 1.
- If [*$A*] ≠ 0, then *$A* becomes 0.

### Flow control

Instruction | Description
----------- | -----------
NOOP | waste some time
HALT | halt the CPU and stop
JMPR _$A_ | jump to memory address stored in *$A*
JMPA _address_ | jump to memory address *address*
JI7R/JI8R/JI9R _$A_ | jump to memory address stored in *$A* if $109/$110/$111 = 0xFF
JI7A/JI8A/JI9A _address_ | jump to *address* if $109/$110/$111 = 0xFF
CALR _$A_ | jump to memory address stored in *$A* and push the next instruction's address into stack
CALA _address_ | jump to *address* and push the next instruction's address into stack
RETN | `POP_ $100`
PUSH _$A_, _length_ | push registers from *$A*...*$(A + length)* onto the stack
POP_ _$A_, _length_ | pop data from stack to registers *$A*...*$(A + length)*

76-Float
--------

76-Float provides instructions for processing floating point values.

You need to set memory *IO + 0x1* (0x400001, one byte) to 0xFF first in order to make it work.

### Basic floating point arithmetic

Instruction | Description
----------- | -----------

76-Vector
---------

76-Vector provides instructions for calculating vectors, just as in the OpenGL shader language.

You need to set memory *IO + 0x2* (0x400002, one byte) to 0xFF first in order to make it work.


### Basic vector arithmetic

Instruction | Description
----------- | -----------

BIOS Instructions
-----------------

**BIOS** is an acronym for Basic Input/Output System, of course.

You need to set memory *IO + 0xA* (0x40000A, one byte) to 0xFF first in order to make it work.

Instruction | Description
----------- | -----------
