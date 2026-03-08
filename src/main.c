#include "instruction.h"

uint8_t program[] = {
    0x10, 0x05,  // LOADS: Reg[0] に 5 を入れる (10=命令, 05=データ)
    0x11, 0x03,  // LOADS: Reg[1] に 3 を入れる (11=命令, 03=データ)
    0x21,        // ADD: A (Reg0) に Reg[1] を足す (20 | 1 = 21)
    0xFF         // HALT: 終了                // 0x1C: HALT             (終了)
};

int main() {
    CPU cpu = {0};
    cpu.sp = 0xFFFF;
    uint8_t memory[65536] = {0};

    memcpy(memory, program, sizeof(program));

    printf("--- Execution Start ---\n");

    while (!cpu.is_halted && cpu.pc < sizeof(memory)) {
        uint8_t opcode = memory[cpu.pc];
        decode_and_run(&cpu, opcode, memory);
    }

    printf("Final Result in Reg[0]: %d\n", cpu.registers[0]);
    return 0;
}