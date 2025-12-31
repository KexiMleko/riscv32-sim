#include <stdint.h>
static inline uint32_t j_opcode(uint32_t instr) { return instr & 0x7F; }
static inline uint32_t j_rd(uint32_t instr) { return (instr >> 7) & 0x1F; }
static inline uint32_t j_imm_u(uint32_t instr) {
    uint32_t imm=instr >> 12;
    uint32_t bit11=(imm>>8)&0x1;
    uint32_t bit20=imm>>19;
    uint32_t bits1912=imm & 0xFF;
    uint32_t bits101=(imm >>9)&0x3FF;
    return (bit20<<19)|(bits1912<<10)|(bit11<<9)|(bits101<<1);
}
