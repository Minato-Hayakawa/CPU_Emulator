#include <string.h>
#include "C:/vscode/c/CPU_Emulator/include/instruction.h"

int main() {
    CPU cpu = {0};
    cpu.sp = 0xFFFF;
    uint8_t memory[65536] = {0};
    uint16_t addr = 0;
    char input[16];

    printf("--- Machine Code Input Mode ---\n");
    printf("Enter hex values (e.g., 10 05 11 03 21 FF). Type 'run' to execute.\n");

    while (addr < 65536) {
        printf("mem[0x%04X] > ", addr);
        if (scanf("%s", input) == EOF || strcmp(input, "run") == 0) {
            break;
        }

        unsigned int value;
        if (sscanf(input, "%x", &value) == 1) {
            memory[addr++] = (uint8_t)value;
        } else {
            printf("Invalid input. Use hex (e.g., FF) or 'run'.\n");
        }
    }

    printf("\n--- Execution Start ---\n");
    cpu.pc = 0; // プログラムの先頭から開始

    while (!cpu.is_halted && cpu.pc < addr) {
        uint8_t opcode = memory[cpu.pc];
        // デバッグ用に現在のPCと読み取った命令を表示すると分かりやすい
        printf("[PC: 0x%04X] Opcode: 0x%02X -> ", cpu.pc, opcode);
        decode_and_run(&cpu, opcode, memory);
    }

    printf("\n--- Execution Finished ---\n");
    printf("Final Registers: R0:%d, R1:%d, R2:%d, R3:%d\n", 
            cpu.registers[0], cpu.registers[1], cpu.registers[2], cpu.registers[3]);
    
    return 0;
}