#include "common_data_bus/common_data_bus.h"
#include "regfile/tomasulo_regfile.h"
#include "reservation_station/reservation_station.h"
#include <stdio.h>

void write_result(CDB cdb) {
  if (!cdb.valid) {
    printf("[WB] CDB idle\n");
    return;
  }

  printf("[WB] CDB broadcast: tag=%u value=%d\n", cdb.tag, cdb.value);
  regfile_cdb_snoop(cdb);
  rs_snoop_cdb(cdb);
}
