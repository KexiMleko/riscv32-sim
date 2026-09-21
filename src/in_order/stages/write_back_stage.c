#include "common/pipe_regs.h"
#include "regfile/regfile.h"
#include "stages/pipeline.h"
#include <stdint.h>
#include "trace.h"

halt_signal write_back(MEM_WB in) {
  if (in.halt_signal) {
    trace_note(TR_WB, "halt");
    return true;
  }
  if (in.instr)
    trace_stat_retire();
  if (in.ctrl.rd_we) {
    uint32_t wr_val = in.ctrl.mem_to_reg ? in.data_o : in.alu_res;
    rd_write(wr_val, in.rd_addr);
    if (in.rd_addr != 0)
      trace_note(TR_WB, "x%u ← %d", in.rd_addr, (int32_t)wr_val);
  }
  return false;
}
