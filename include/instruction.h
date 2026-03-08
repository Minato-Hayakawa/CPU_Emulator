#include "cpu.h"
void execute_cycle(CPU *cpu, uint8_t *memory);
void decode_and_run(CPU *cpu, uint8_t opcode, uint8_t *memory);
void update_flags(CPU *cpu, uint8_t result);