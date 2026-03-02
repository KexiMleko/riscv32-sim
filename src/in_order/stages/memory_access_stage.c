#include "common/pipe_regs.h"
#include "mem_addressing_ctrl.h"
#include "memory/memory.h"
#include "stages/pipeline.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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
  if (in.ctrl.data_mem_read_en || in.ctrl.data_mem_write_en)
    printf("Memory access - %s at address 0x%x, value: %d\n",
           in.ctrl.data_mem_write_en ? "Write" : "Read", in.alu_res,
           in.ctrl.data_mem_write_en ? in.data_i : out.data_o);

  return out;
}
