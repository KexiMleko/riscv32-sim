#include "common_data_bus/common_data_bus.h"
#include "regfile/tomasulo_regfile.h"
#include "reservation_station/reservation_station.h"
#include <stdio.h>

void write_result(CDB cdb, reservation_station reservation_stations[],
                  unsigned rs_cnt) {
  if (!cdb.valid) {
    printf("[WB] CDB idle\n");
    return;
  }

  printf("[WB] CDB broadcast: tag=%u value=%d\n", cdb.tag, cdb.value);
  regfile_cdb_snoop(cdb);

  // reservation station snoop
  for (unsigned i = 0; i < rs_cnt; i++) {
    if (!reservation_stations[i].busy)
      continue;

    if (reservation_stations[i].rs1_tag == cdb.tag) {
      reservation_stations[i].rs1_val = cdb.value;
      reservation_stations[i].rs1_tag = 0;
      printf("[WB]  RS[%u] rs1 resolved (val=%d)\n", i, cdb.value);
    }
    if (reservation_stations[i].rs2_tag == cdb.tag) {
      reservation_stations[i].rs2_val = cdb.value;
      reservation_stations[i].rs2_tag = 0;
      printf("[WB]  RS[%u] rs2 resolved (val=%d)\n", i, cdb.value);
    }
  }
}
