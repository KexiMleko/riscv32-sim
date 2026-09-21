#include "alu.h"
#include <stdio.h>

uint32_t execute_alu(uint32_t value1, uint32_t value2, alu_op_t op) {

  switch (op) {
  case ALU_ADD:
    return value1 + value2;
  case ALU_SUB:
    return value1 - value2;
  case ALU_AND:
    return value1 & value2;
  case ALU_OR:
    return value1 | value2;
  case ALU_XOR:
    return value1 ^ value2;
  case ALU_SLL:
    return value1 << (value2 & 0x1F);
  case ALU_SRL:
    return value1 >> (value2 & 0x1F);
  case ALU_SRA:
    return (int32_t)value1 >> (value2 & 0x1F);
  case ALU_SLT:
    return ((int32_t)value1 < (int32_t)value2) ? 1 : 0;
  case ALU_SLTU:
    return (value1 < value2) ? 1 : 0;
  case ALU_PASS:
    return value1;
  case ALU_NOP:
    return 0;
  case ALU_INVALID:
    fprintf(stderr, "Invalid ALU operation invoked\n");
    return 0;
  default:
    fprintf(stderr, "Unkown ALU operation invoked\n");
    return 0;
  }
}

const char *alu_op_str(alu_op_t op) {
  switch (op) {
  case ALU_ADD:
    return "add";
  case ALU_SUB:
    return "sub";
  case ALU_AND:
    return "and";
  case ALU_OR:
    return "or";
  case ALU_XOR:
    return "xor";
  case ALU_SLL:
    return "sll";
  case ALU_SRL:
    return "srl";
  case ALU_SRA:
    return "sra";
  case ALU_SLT:
    return "slt";
  case ALU_SLTU:
    return "sltu";
  case ALU_PASS:
    return "pass";
  case ALU_NOP:
    return "nop";
  default:
    return "?";
  }
}
