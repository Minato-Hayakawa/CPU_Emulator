# 8-bit Custom CPU Emulator in C

A lightweight, functional 8-bit CPU emulator implemented in C. This project simulates a custom Instruction Set Architecture (ISA) featuring a classical Fetch-Decode-Execute cycle, stack operations, and flag-based conditional branching.

---

## Architecture Overview
This emulator implements a custom 8-bit architecture designed for educational purposes. It mimics the behavior of early microprocessors, handling 8-bit data with a 16-bit address space.

### Specifications
* **Word Size**: 8-bit
* **Address Space**: 16-bit (64 KB)
* **Registers**: 
    * 8 General-purpose registers (`registers[0-7]`)
    * 16-bit Program Counter (`PC`)
    * 16-bit Stack Pointer (`SP`)
* **Flags**: Zero Flag (`Z`) and Sign Flag (`S`)
* **Memory**: 65,536 bytes (Flat memory model)

---

## Instruction Set Architecture (ISA)
The CPU uses a variable-length instruction format. The **high 4 bits** of the opcode define the instruction type, while the **low 4 bits** typically specify the target register index.

| Opcode (Hex) | Instruction | Description |
| :--- | :--- | :--- |
| `0x1R` | `LOADS Reg[R], imm` | Load an 8-bit immediate value into register R. |
| `0x2R` | `MOV Reg[R], Reg[src]` | Copy value from source register to destination register. |
| `0x3R` | `STORE Reg[R], [addr]` | Store register value to a 16-bit memory address. |
| `0x4R` | `PUSH Reg[R]` | Push register value onto the stack (SP decrements). |
| `0x5R` | `POP Reg[R]` | Pop value from the stack into register R (SP increments). |
| `0x6R` | `ADD A, Reg[R]` | Add Reg[R] to Reg[0] (Accumulator). Updates flags. |
| `0x7R` | `SUB A, Reg[R]` | Subtract Reg[R] from Reg[0]. Updates flags. |
| `0x8R` | `MUL A, Reg[R]` | Multiply Reg[0] by Reg[R]. Updates flags. |
| `0x9R` | `DIV A, Reg[R]` | Divide Reg[0] by Reg[R]. Includes zero-check. |
| `0xA0` | `JMP addr` | Unconditional jump to a 16-bit address. |
| `0xB0` | `JZ addr` | Jump to 16-bit address if Zero Flag is set. |
| `0xFF` | `HALT` | Terminate CPU execution. |

---

## Key Implementation Details

### 1. The Fetch-Decode-Execute Cycle
The core logic resides in `decode_and_run`. It efficiently dispatches instructions by masking bits to separate the operation from the operand:
```c
uint8_t type = (opcode & 0xF0);
uint8_t reg_idx = (opcode & 0x0F);

### 2. Flag Management
After arithmetic operations, the CPU state is updated via `update_flags`. This is critical for implementing loops and conditional logic:
* **Zero Flag (Z)**: Set to 1 if the result of an operation is 0.
* **Sign Flag (S)**: Set to 1 if the MSB (Most Significant Bit) is 1, indicating a negative value in two's complement representation.

### 3. Stack Mechanism
The emulator simulates a downward-growing stack, typical of classic architectures like the x86 or Z80.
* **Push**: The `SP` (Stack Pointer) is decremented, and the register value is written to `memory[SP]`.
* **Pop**: The value is read from `memory[SP]`, and the `SP` is then incremented.

---

## Sample Program: Fibonacci Sequence
The project includes a pre-defined machine code array (`Fibonacci[]`) to calculate the sequence. This demonstrates the power of the custom ISA:
1. **Initialization**: Loading starting values (0 and 1) into registers.
2. **Looping**: Using arithmetic instructions to calculate the next number.
3. **Control Flow**: Using `JZ` (Jump if Zero) and `JMP` (Unconditional Jump) to manage the loop logic and termination.

---

## How to Build and Run
1. Ensure you have a C compiler (e.g., GCC or Clang) installed.
2. Save the source code (including `cpu.h` and `instruction.h` if separated).
3. Compile the source:
   ```bash
   make

3. Run the executable:
   ```bash
   .\emulator.exe
