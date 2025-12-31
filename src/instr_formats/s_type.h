#include <stdint.h>
#include <stdio.h>
static inline uint32_t s_opcode(uint32_t instr){
    return instr & 0x7F;
}
static inline uint32_t s_funct3(uint32_t instr){
    return (instr>>12)& 0x07;
}
static inline uint32_t s_rs1(uint32_t instr){
    return (instr>>15)& 0x1F;
}
static inline uint32_t s_rs2(uint32_t instr){
    return (instr>>20)& 0x1F;
}
static inline uint32_t s_imm_u(uint32_t instr){
    uint32_t lower_five= (instr>>7) & 0x1F;
    uint32_t upper_seven= (instr>>25)<<5;
    return upper_seven|lower_five;
}
