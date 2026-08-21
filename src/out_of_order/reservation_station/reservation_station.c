#include "reservation_station.h"
#include "common_data_bus/common_data_bus.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

#define RS_CNT 8

static reservation_station reservation_stations[RS_CNT] = {0};

void rs_snoop_cdb(CDB cdb) {
  // reservation station snoop
  for (unsigned i = 0; i < RS_CNT; i++) {
    if (!reservation_stations[i].busy)
      continue;

    if (reservation_stations[i].rs1_tag == cdb.tag) {
      reservation_stations[i].rs1_val = cdb.value;
      reservation_stations[i].rs1_tag = 0;
      printf("[WB]  RS[%u] rs1 resolved (val=%d)\n", i + 1, cdb.value);
    }
    if (reservation_stations[i].rs2_tag == cdb.tag) {
      reservation_stations[i].rs2_val = cdb.value;
      reservation_stations[i].rs2_tag = 0;
      printf("[WB]  RS[%u] rs2 resolved (val=%d)\n", i + 1, cdb.value);
    }
  }
}

void rs_free(uint32_t tag) { reservation_stations[tag - 1].busy = false; }

uint32_t rs_find_free_tag() {
  for (unsigned i = 0; i < RS_CNT; i++) {
    if (!reservation_stations[i].busy) {
      return i + 1;
    }
  }
  return 0;
}
uint32_t rs_find_ready(reservation_station *rs) {
  for (size_t i = 0; i < RS_CNT; i++) {
    if (reservation_stations[i].busy) {
      reservation_station rs_temp = reservation_stations[i];
      if (rs_temp.rs1_tag == 0 && rs_temp.rs2_tag == 0) {
        *rs = rs_temp;
        return i + 1;
      } else {
        printf("[EX] RS[%zu] waiting: rs1_tag=%u rs2_tag=%u\n", i + 1,
               rs_temp.rs1_tag, rs_temp.rs2_tag);
      }
    }
  }
  return 0;
}
bool rs_update(uint32_t tag, reservation_station rs_new) {
  if (tag < RS_CNT) {
    reservation_stations[tag - 1] = rs_new;
    return true;
  }
  return false;
}
bool rs_all_empty() {
  for (size_t i = 0; i < RS_CNT; i++) {
    if (reservation_stations[i].busy)
      return false;
  }
  return true;
}
