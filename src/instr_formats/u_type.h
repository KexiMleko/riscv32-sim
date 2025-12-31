#include <stdint.h>
static inline uint32_t u_opcode(uint32_t instr){
    return instr & 0x7F;
}
static inline uint32_t u_rd(uint32_t instr){
    return (instr>>7)& 0x1F;
}
static inline uint32_t u_imm_u(uint32_t instr){
    return instr>>12;
}
