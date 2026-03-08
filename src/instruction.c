#include "instruction.h"

void execute_cycle(CPU *cpu, uint8_t *memory) {
    // 1. Fetch: 命令の読み込み
    uint8_t opcode = memory[cpu->pc];

    // 2. Decode & Execute: 命令の解釈と実行
    decode_and_run(cpu, opcode, memory);

    // 3. 次の命令へ
}

void update_flags(CPU *cpu, uint8_t result) {
    cpu->z_flag = (result == 0) ? 1 : 0;
    cpu->s_flag = (result & 0x80) ? 1 : 0; // 最上位ビットが1ならマイナス
}

void decode_and_run(CPU *cpu, uint8_t opcode, uint8_t *memory) {
    //上位4ビットを取り出す
    uint8_t type = (opcode & 0xF0);
    // 下位4ビットを取り出す
    // 例: 0x1R (Rはレジスタ番号) -> MOV Reg[R], imm
    uint8_t reg_idx = (opcode & 0x0F); 
    uint16_t current_pc = cpu->pc;

    switch (type) {
        // --- データ転送系 (0x10 ~ 0x14) ---
        case 0x10: // LOADS
            if (reg_idx < 4) { //レジスタの範囲をチェック
                cpu->pc++;
                cpu->registers[reg_idx] = memory[cpu->pc];
                printf("MOV Reg[%d], %d\n", reg_idx, cpu->registers[reg_idx]);
                cpu->pc++;
            }
            break;
        case 0x11: // MOV Reg[R], Reg[src] 
        {
            cpu->pc++;
            uint8_t src_idx = memory[cpu->pc];
            if (reg_idx < 4 && src_idx < 4) {
                cpu->registers[reg_idx] = cpu->registers[src_idx];               
            }
            cpu->pc++;
        }
        break;
        case 0x12: // STORE Reg[R], [addr] (レジスタをメモリへ)
        {
            cpu->pc++;
            // アドレスは2バイト(16bit)必要
            uint16_t addr = (memory[cpu->pc] << 8) | memory[cpu->pc + 1];
            cpu->pc += 2;
            memory[addr] = cpu->registers[reg_idx];
        }
        break;
        case 0x13: // PUSH Reg[R]
        if (reg_idx < 4) {
            cpu->pc++;
            cpu->sp--; 
            memory[cpu->sp] = cpu->registers[reg_idx];
            printf("PUSH Reg[%d] to 0x%04X\n", reg_idx, cpu->sp);
        }
        break;
        case 0x14: // POP Reg[R]
        if (reg_idx < 4) {
            cpu->pc++;
            cpu->registers[reg_idx] = memory[cpu->sp];
            cpu->sp++; 
            printf("POP Reg[%d] from 0x%04X\n", reg_idx, cpu->sp - 1);
        }
        break;
    // --- 算術演算系 (0x20系) ---
        case 0x20: // ADD A, Reg[R] (Aレジスタに他のレジスタを足す)
        {
            if (reg_idx < 4) {
                cpu->pc++;
                cpu->registers[0] += cpu->registers[reg_idx];
                update_flags(cpu, cpu->registers[0]);
                printf("ADD A, Reg[%d] (Result: %d)\n", reg_idx, cpu->registers[0]);
            }
        }
        break;

        case 0x21: // SUB A, Reg[R]
        {
            if (reg_idx < 4) {
                cpu->pc++;
                cpu->registers[0] -= cpu->registers[reg_idx];
                update_flags(cpu, cpu->registers[0]);
            }
        }
        break;
        case 0x22: //MUL A, Reg[R]
        {
            if (reg_idx < 4){
                cpu->pc++;
                cpu->registers[0] *= cpu->registers[reg_idx];
                update_flags(cpu, cpu->registers[0]);
            }
        }
        break;
        case 0x23: //DIV A, Reg[R]
        {
            cpu->pc++;
            if (reg_idx < 4 && cpu->registers[reg_idx] != 0){
                cpu->registers[0] /= cpu->registers[reg_idx];
                update_flags(cpu, cpu->registers[0]);
            }else{
                printf("cannot div %d !!", cpu->registers[reg_idx]);
            }
        }
        break;

        // --- 制御系 (0x30系) ---
        case 0x30: // JMP addr
            cpu->pc++;    
            cpu->pc = (memory[cpu->pc] << 8) | memory[cpu->pc + 1];
            return; 

        case 0x40: // JZ addr
            cpu->pc++; 
            if (cpu->z_flag) {
                cpu->pc = (memory[cpu->pc] << 8) | memory[cpu->pc + 1];
                return; 
            } else {
                cpu->pc += 2;
                break;
            }
        case 0xF0: // HALT
            if (opcode == 0xFF) {
                cpu->pc++; // HALT命令分進める
                cpu->is_halted = 1;
                printf("--- HALT ---\n");
            } else {
                printf("Unknown Opcode in F-range: 0x%02X\n", opcode);
                cpu->pc++;
            }
            break;

        default:
            printf("Unknown Opcode: 0x%02X\n", opcode);
            cpu->pc++;
            break;
    }
}