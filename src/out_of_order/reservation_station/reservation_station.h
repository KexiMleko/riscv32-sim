#pragma once

#include "alu_op.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  alu_op_t op;
  uint32_t rs1_val;
  uint32_t rs2_val;
  int rs1_tag;
  int rs2_tag;
  uint32_t addr;
  bool busy;
} reservation_station;
