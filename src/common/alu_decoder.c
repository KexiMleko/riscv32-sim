#include "alu_decoder.h"
#include <stdint.h>

alu_op_t op_alu_decode(uint32_t funct3, uint32_t funct7) {
  switch ((funct7 << 3) | funct3) {
  case (0x00 << 3) | 0x0:
    return ALU_ADD;
  case (0x20 << 3) | 0x0:
    return ALU_SUB;
  case (0x00 << 3) | 0x1:
    return ALU_SLL;
  case (0x00 << 3) | 0x2:
    return ALU_SLT;
  case (0x00 << 3) | 0x3:
    return ALU_SLTU;
  case (0x00 << 3) | 0x4:
    return ALU_XOR;
  case (0x00 << 3) | 0x5:
    return ALU_SRL;
  case (0x20 << 3) | 0x5:
    return ALU_SRA;
  case (0x00 << 3) | 0x6:
    return ALU_OR;
  case (0x00 << 3) | 0x7:
    return ALU_AND;
  default:
    return ALU_INVALID;
  }
}
alu_op_t op_imm_alu_decode(uint32_t funct3, uint32_t funct7) {
  switch (funct3) {
  case 0x0:
    return ALU_ADD;
  case 0x1:
    return ALU_SLL;
  case 0x2:
    return ALU_SLT;
  case 0x3:
    return ALU_SLTU;
  case 0x4:
    return ALU_XOR;
  case 0x5:
    if (funct7 == 0x20) {
      return ALU_SRA;
    } else {
      return ALU_SRL;
    }
  case 0x6:
    return ALU_OR;
  case 0x7:
    return ALU_AND;
  default:
    return ALU_INVALID;
  }
}
