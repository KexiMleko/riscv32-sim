#include "alu.h"
#include "ooo_pipeline.h"
#include "reservation_station/reservation_station.h"
#include <stddef.h>
#include <stdio.h>

CDB ooo_execute(reservation_station *reservation_stations, unsigned rs_cnt) {
  CDB res = {0};
  printf("[EX] scanning %u reservation stations\n", rs_cnt);
  for (size_t i = 0; i < rs_cnt; i++) {
    if (reservation_stations[i].busy) {
      reservation_station *rs = &reservation_stations[i];
      if (rs->rs1_tag == 0 && rs->rs2_tag == 0) {
        res.value = execute_alu(rs->rs1_val, rs->rs2_val, rs->op);
        res.valid = true;
        res.tag = i;
        printf("[EX] RS[%zu] ready: op=0x%02x rs1=%d rs2=%d -> result=%d (tag=%u)\n",
               i, rs->op, rs->rs1_val, rs->rs2_val, res.value, res.tag);
        return res;
      } else {
        printf("[EX] RS[%zu] waiting: rs1_tag=%u rs2_tag=%u\n",
               i, rs->rs1_tag, rs->rs2_tag);
      }
    }
  }
  printf("[EX] no RS ready this cycle\n");
  return res;
}
