#include "common_data_bus/common_data_bus.h"
#include "halt_signal.h"
#include "memory/memory.h"
#include "pipe_regs.h"
#include "reservation_station/reservation_station.h"

typedef struct {
  bool valid;
  uint8_t tag;
  reservation_station rs;
  uint32_t rd; // register destination for regfile update
} issue_result;

IF_ID ooo_instr_fetch(IF_ID in, instr_memory *instr_mem, uint32_t pc);
issue_result issue_instr(CDB cdb);
CDB ooo_execute();
halt_signal write_result(CDB cdb, bool halt_pending);
