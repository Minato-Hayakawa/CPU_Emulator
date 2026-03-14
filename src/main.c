#include "instruction.h"

uint8_t program[] = {
    0x10, 0x05,  // LOADS: Reg[0] に 5 を入れる (10=命令, 05=データ)
    0x11, 0x03,  // LOADS: Reg[1] に 3 を入れる (11=命令, 03=データ)
    0x61,        // ADD: A (Reg0) に Reg[1] を足す (20 | 1 = 21)
    0xFF         // HALT: 終了                // 0x1C: HALT             (終了)
};

uint8_t Fibonacci[] = {
    0x10, 0x00, //レジスタ0に0を代入
    0x11, 0x01,//レジスタ1に1を代入
    0x13, 0x0A,//レジスタ3に10を代入
    0x61,//レジスタ0とレジスタ1の値を加算し、レジスタ0に代入
    0x22, 0x00,//レジスタ0の値をレジスタ2にコピー
    0x20, 0x03,
    0x14, 0x01,
    0x74,
    0xB0,//レジスタ0の値が0でないなら続行
    0xA0, 0x00, 0x06, //6番目の指令に戻る
    0xFF
}

int main() {
    CPU cpu = {0};
    cpu.sp = 0xFFFF;

    // 1. memory_fft という名前で、メモリと同じサイズ (65536) の配列を作る
    // 括弧内で直接、実行したい命令（マシン語）を書いていきます。
    uint8_t memory_fft[65536] = {
        // --- 複素数 (8 + 2i) + (5 + 3i) を計算する例 ---
        // 実部の計算: 8 + 5
        0x10, 0x08,  // LOADS Reg[0], 8  (Reg[0]を実部 accumulator とする)
        0x11, 0x05,  // LOADS Reg[1], 5  (Reg[1]に足したい実部を入れる)
        0x21,        // ADD A, Reg[1]    (Reg[0] = 8 + 5 = 13)
        0x12, 0x01,
        0x00,  // STORE Reg[0], [0x0100] (実部の結果13をメモリ0x0100に保存)

        // 虚部の計算: 2 + 3
        0x10, 0x02,  // LOADS Reg[0], 2  (Reg[0]に次の計算用数値を入れる)
        0x11, 0x03,  // LOADS Reg[1], 3
        0x21,        // ADD A, Reg[1]    (Reg[0] = 2 + 3 = 5)
        0x12, 0x01,
        0x01,  // STORE Reg[0], [0x0101] (虚部の結果5をメモリ0x0101に保存)

        0xFF  // HALT (終了)
    };

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