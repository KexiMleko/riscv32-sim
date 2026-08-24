#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint32_t next_pc;
  bool pc_next_sel;
} branch_ctrl;

bool eval_branch(uint32_t rs1_val, uint32_t rs2_val, uint8_t funct3);
