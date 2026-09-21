#include "common/pipe_regs.h"
#include "mem_addressing_ctrl.h"
#include "memory/memory.h"
#include "stages/pipeline.h"
#include <stdbool.h>
#include <stdint.h>
#include "trace.h"

MEM_WB memory_access(EX_MEM in, data_memory *data_mem) {

  MEM_WB out = {0};

  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }

  bool zero_extend = in.ctrl.mem_addressing_ctrl.zero_extend;
  mem_width_t width = in.ctrl.mem_addressing_ctrl.addressing_width;

  if (in.ctrl.data_mem_write_en) {
    data_mem_write(in.data_i, in.alu_res, width, data_mem);
  }

  out.alu_res = in.alu_res;
  out.ctrl = in.ctrl;

  if (in.ctrl.data_mem_read_en) {
    out.data_o = data_mem_read(in.alu_res, zero_extend, width, data_mem);
  }

  out.rd_addr = in.rd_addr;
  out.instr = in.instr;
  if (in.ctrl.data_mem_write_en)
    trace_note(TR_MEM, "W [0x%04x] ← %d", in.alu_res, (int32_t)in.data_i);
  else if (in.ctrl.data_mem_read_en)
    trace_note(TR_MEM, "R [0x%04x] → %d", in.alu_res, (int32_t)out.data_o);

  return out;
}
