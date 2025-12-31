#include <stdint.h>
static inline uint32_t b_opcode(uint32_t instr){
    return instr & 0x7F;
}
static inline uint32_t b_funct3(uint32_t instr){
    return (instr>>12)& 0x07;
}
static inline uint32_t b_rs1(uint32_t instr){
    return (instr>>15)& 0x1F;
}
static inline uint32_t b_rs2(uint32_t instr){
    return (instr>>20)& 0x1F;
}
static inline uint32_t b_imm_u(uint32_t instr){
    uint32_t lower_five= (instr>>7)& 0x1F;
    uint32_t upper_seven= (instr>>25)<<7;
    //12|10:5,4:1|11
    uint32_t msb=upper_seven>>6;
    uint32_t lsb=lower_five>>4;
    lower_five&=~0x1;
    upper_seven&=~0x80;
    msb=((msb<<1)+lsb)<<12;
    return msb+upper_seven+lower_five;
}
