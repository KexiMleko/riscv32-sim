#include "reservation_station.h"
#include "common_data_bus/common_data_bus.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include "alu.h"
#include "trace.h"

static reservation_station reservation_stations[RS_CNT] = {0};

void rs_snoop_cdb(CDB cdb) {
  // reservation station snoop
  for (unsigned i = 0; i < RS_CNT; i++) {
    if (!reservation_stations[i].busy)
      continue;

    if (reservation_stations[i].rs1_tag == cdb.tag) {
      reservation_stations[i].rs1_val = cdb.value;
      reservation_stations[i].rs1_tag = 0;
      trace_note(TR_CDB, "→ RS%u.j", i + 1);
    }
    if (reservation_stations[i].rs2_tag == cdb.tag) {
      reservation_stations[i].rs2_val = cdb.value;
      reservation_stations[i].rs2_tag = 0;
      trace_note(TR_CDB, "→ RS%u.k", i + 1);
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
      }
    }
  }
  return 0;
}
bool rs_update(uint32_t tag, reservation_station rs_new) {
  if (tag == 0 || tag > RS_CNT) {
    return false;
  }
  reservation_stations[tag - 1] = rs_new;
  return true;
}
bool rs_all_empty() {
  for (size_t i = 0; i < RS_CNT; i++) {
    if (reservation_stations[i].busy)
      return false;
  }
  return true;
}

// One line per busy station: operand value or the #tag it is waiting for
void rs_render(char *buf, size_t cap) {
  buf[0] = '\0';
  for (size_t i = 0; i < RS_CNT; i++) {
    const reservation_station *rs = &reservation_stations[i];
    if (!rs->busy)
      continue;
    char j[16], k[16];
    if (rs->rs1_tag)
      snprintf(j, sizeof(j), "#%u", rs->rs1_tag);
    else
      snprintf(j, sizeof(j), "%d", rs->rs1_val);
    if (rs->rs2_tag)
      snprintf(k, sizeof(k), "#%u", rs->rs2_tag);
    else
      snprintf(k, sizeof(k), "%d", rs->rs2_val);
    trace_appendf(buf, cap, "%sRS%zu %-4s j=%-4s k=%-4s %s", buf[0] ? "\n" : "",
                  i + 1, alu_op_str(rs->op), j, k,
                  (rs->rs1_tag || rs->rs2_tag) ? "waiting" : "ready");
  }
}
