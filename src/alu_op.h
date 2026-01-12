#pragma once

typedef enum {
  ALU_ADD,
  ALU_SUB,
  ALU_AND,
  ALU_OR,
  ALU_XOR,
  ALU_SLL,
  ALU_SRL,
  ALU_SRA,
  ALU_SLT,
  ALU_SLTU,
  ALU_PASS,
  ALU_NOP,
  ALU_INVALID
} alu_op_t;
