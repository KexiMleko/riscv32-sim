#pragma once

#include "alu_op.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  alu_op_t op;
  int32_t rs1_val;
  int32_t rs2_val;
  unsigned rs1_tag;
  unsigned rs2_tag;
  uint32_t addr;
  bool busy;
} reservation_station;
