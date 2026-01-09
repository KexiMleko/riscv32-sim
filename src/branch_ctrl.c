#include "branch_ctrl.h"
#include <stdbool.h>
#include <stdint.h>

bool eval_branch(uint32_t rs1_val, uint32_t rs2_val, uint8_t funct3) {
  switch (funct3) {
  case 0b000:
    return rs1_val == rs2_val; // BEQ
  case 0b001:
    return rs1_val != rs2_val; // BNE
  case 0b100:
    return (int32_t)rs1_val < (int32_t)rs2_val; // BLT
  case 0b101:
    return (int32_t)rs1_val >= (int32_t)rs2_val; // BGE
  case 0b110:
    return rs1_val < rs2_val; // BLTU
  case 0b111:
    return rs1_val >= rs2_val; // BGEU
  default:
    return false;
  }
}
