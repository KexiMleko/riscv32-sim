#include "alu.h"
#include "ooo_pipeline.h"
#include "reservation_station/reservation_station.h"
#include <stddef.h>
#include <stdio.h>

CDB ooo_execute() {
  CDB res = {0};
  printf("[EX] scanning reservation stations\n");

  reservation_station rs;
  uint32_t ready_tag = rs_find_ready(&rs);
  if (!ready_tag) {
    printf("[EX] no RS ready this cycle\n");
    return res;
  }
  res.value = execute_alu(rs.rs1_val, rs.rs2_val, rs.op);
  res.valid = true;
  res.tag = ready_tag;

  printf("[EX] RS[%u] ready: op=0x%02x rs1=%d rs2=%d -> result=%d (tag=%u)\n",
         ready_tag, rs.op, rs.rs1_val, rs.rs2_val, res.value, res.tag);
  return res;
}
