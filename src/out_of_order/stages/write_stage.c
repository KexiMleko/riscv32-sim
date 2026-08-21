#include "common_data_bus/common_data_bus.h"
#include "halt_signal.h"
#include "instruction_queue/instr_queue.h"
#include "regfile/tomasulo_regfile.h"
#include "reservation_station/reservation_station.h"
#include <stdio.h>

halt_signal write_result(CDB cdb, bool halt_pending) {
  halt_signal halt = halt_pending && iq_is_empty() && rs_all_empty();
  if (!cdb.valid) {
    printf("[WB] CDB idle\n");
    return halt;
  }

  printf("[WB] CDB broadcast: tag=%u value=%d\n", cdb.tag, cdb.value);
  regfile_cdb_snoop(cdb);
  rs_snoop_cdb(cdb);
  return halt;
}
