#include <stdint.h>
static inline uint32_t r_opcode(uint32_t instr){
    return instr & 0x7F;
}
static inline uint32_t r_rd(uint32_t instr){
    return (instr>>7)& 0x1F;
}
static inline uint32_t r_funct3(uint32_t instr){
    return (instr>>12)& 0x07;
}
static inline uint32_t r_rs1(uint32_t instr){
    return (instr>>15)& 0x1F;
}
static inline uint32_t r_rs2(uint32_t instr){
    return (instr>>20)& 0x1F;
}
static inline uint32_t r_funct7(uint32_t instr){
    return instr>>25;
}
