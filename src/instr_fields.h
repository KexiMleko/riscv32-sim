#include <stdint.h>
static inline uint32_t opcode(uint32_t instr) { return instr & 0x7F; }
static inline uint32_t rd(uint32_t instr) { return (instr >> 7) & 0x1F; }
static inline uint32_t funct3(uint32_t instr) { return (instr >> 12) & 0x07; }
static inline uint32_t rs1(uint32_t instr) { return (instr >> 15) & 0x1F; }
static inline uint32_t rs2(uint32_t instr) { return (instr >> 20) & 0x1F; }
static inline uint32_t funct7(uint32_t instr) { return instr >> 25; }
static inline int32_t i_imm(uint32_t instr) { return (int32_t)(instr >> 20); }
static inline int32_t j_imm(uint32_t instr) {
  int32_t imm = 0;
  imm |= ((instr >> 31) & 0x1) << 20 | ((instr >> 12) & 0xFF) << 12 |
         ((instr >> 20) & 0x1) << 11 | ((instr >> 21) & 0x3FF) << 1;
  return (int32_t)(imm << 11) >> 11;
}
static inline int32_t u_imm(uint32_t instr) { return (int32_t)instr >> 12; }
static inline int32_t b_imm(uint32_t instr) {
  uint32_t lower_five = (instr >> 7) & 0x1F;
  uint32_t upper_seven = (instr >> 25) << 7;
  uint32_t msb = upper_seven >> 6;
  uint32_t lsb = lower_five >> 4;
  lower_five &= ~0x1;
  upper_seven &= ~0x80;
  msb = ((msb << 1) | lsb) << 12;
  return (uint32_t)(msb | upper_seven | lower_five);
}
static inline int32_t s_imm(uint32_t instr) {
  uint32_t lower_five = (instr >> 7) & 0x1F;
  uint32_t upper_seven = (instr >> 25) << 5;
  return (int32_t)(upper_seven | lower_five);
}
