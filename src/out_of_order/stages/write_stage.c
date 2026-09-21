#include "common_data_bus/common_data_bus.h"
#include "halt_signal.h"
#include "instruction_queue/instr_queue.h"
#include "ls_buffers/load_buffers.h"
#include "ls_buffers/store_buffers.h"
#include "regfile/tomasulo_regfile.h"
#include "reservation_station/reservation_station.h"
#include "trace.h"

halt_signal write_result(CDB cdb, bool halt_pending) {
  halt_signal halt = halt_pending && iq_is_empty() && rs_all_empty() && lb_all_empty() && sb_all_empty();
  if (!cdb.valid) {
    return halt;
  }

  trace_row(TR_CDB, "#%u = %d", cdb.tag, cdb.value);
  regfile_cdb_snoop(cdb);
  rs_snoop_cdb(cdb);
  sb_snoop_cdb(cdb);
  return halt;
}
