#include "common_data_bus/common_data_bus.h"
#include "control_decoder.h"
#include "halt_signal.h"
#include "ls_buffers/load_buffers.h"
#include "ls_buffers/store_buffers.h"
#include "memory/memory.h"
#include "pipe_regs.h"
#include "reservation_station/reservation_station.h"

typedef struct {
  bool valid;
  uint8_t tag;
  control_signals ctrl;
  reservation_station rs;
  load_buffer lb;
  store_buffer sb;
  uint32_t rd; // register destination for regfile update
} issue_result;

typedef enum { UNIT_NONE, UNIT_RS, UNIT_LB, UNIT_SB } exec_unit;

typedef struct {
  CDB cdb;
  exec_unit freed_unit;
  uint32_t freed_tag;
} exec_result;

IF_ID ooo_instr_fetch(IF_ID in, instr_memory *instr_mem, uint32_t pc);
issue_result issue_instr(CDB cdb);
exec_result ooo_execute(data_memory *dmem);
halt_signal write_result(CDB cdb, bool halt_pending);
