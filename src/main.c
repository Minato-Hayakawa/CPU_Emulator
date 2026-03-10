#include "instruction.h"

int main() {
    CPU cpu = {0};
    cpu.sp = 0xFFFF;  // スタックポインタの初期化

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

    // 2. CPUが読み取れるように実際の「メモリ」にコピーする
    uint8_t memory[65536] = {0};
    memcpy(memory, memory_fft, sizeof(memory_fft));

    printf("--- Complex Calculation Start ---\n");

    // 3. 実行ループ
    while (!cpu.is_halted && cpu.pc < sizeof(memory)) {
        uint8_t opcode = memory[cpu.pc];
        decode_and_run(&cpu, opcode, memory);  // 命令の解釈と実行
    }

    // 最後にメモリの中身（計算結果）を表示して確認
    printf("Result (Real part at [0x0100]): %d\n", memory[0x0100]);
    printf("Result (Imag part at [0x0101]): %d\n", memory[0x0101]);

    return 0;
}