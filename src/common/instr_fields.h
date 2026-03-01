#pragma once
#include <stdint.h>
static inline uint32_t get_opcode(uint32_t instr) { return instr & 0x7F; }
static inline uint32_t get_rd(uint32_t instr) { return (instr >> 7) & 0x1F; }
static inline uint32_t get_funct3(uint32_t instr) { return (instr >> 12) & 0x07; }
static inline uint32_t get_rs1(uint32_t instr) { return (instr >> 15) & 0x1F; }
static inline uint32_t get_rs2(uint32_t instr) { return (instr >> 20) & 0x1F; }
static inline uint32_t get_funct7(uint32_t instr) { return instr >> 25; }
static inline int32_t get_i_imm(uint32_t instr) { return (int32_t)(instr >> 20); }
static inline int32_t get_j_imm(uint32_t instr) {
  int32_t imm = 0;
  imm |= ((instr >> 31) & 0x1) << 20 | ((instr >> 12) & 0xFF) << 12 |
         ((instr >> 20) & 0x1) << 11 | ((instr >> 21) & 0x3FF) << 1;
  return (int32_t)(imm << 11) >> 11;
}
static inline int32_t get_u_imm(uint32_t instr) { return (int32_t)instr >> 12; }
static inline int32_t get_b_imm(uint32_t instr) {
    int32_t imm = 0;
    uint32_t b12 = (instr >> 31) & 0x1;  
    uint32_t b11 = (instr >> 7)  & 0x1; 
    uint32_t b10_5 = (instr >> 25) & 0x3F;
    uint32_t b4_1 = (instr >> 8)  & 0xF;
    imm = (b12 << 12) | (b11 << 11) | (b10_5 << 5) | (b4_1 << 1);

    if (imm & 0x1000) {
        imm |= 0xFFFFE000;
    }
    return imm;
}
static inline int32_t get_s_imm(uint32_t instr) {
  uint32_t lower_five = (instr >> 7) & 0x1F;
  uint32_t upper_seven = (instr >> 25) << 5;
  return (int32_t)(upper_seven | lower_five);
}
