#pragma once

#include "alu_op.h"
#include "common_data_bus/common_data_bus.h"
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

void rs_snoop_cdb(CDB cdb);
void rs_free(uint32_t tag);
uint32_t rs_find_free_tag();
uint32_t rs_find_ready(reservation_station *rs);
bool rs_update(uint32_t tag, reservation_station rs_new);
bool rs_all_empty();
