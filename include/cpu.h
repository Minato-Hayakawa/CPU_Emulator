#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    uint8_t  registers[8]; // 8個の汎用レジスタ
    uint16_t pc;           // プログラムカウンタ
    uint16_t sp;           // スタックポインタ
    uint8_t  s_flag;        // ゼロフラグなど
    uint8_t z_flag;
    int is_halted;
} CPU;