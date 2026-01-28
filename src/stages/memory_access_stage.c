#include "../pipe_regs.h"
#include "pipeline.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

extern uint32_t data_mem[1024];
MEM_WB memory_access(EX_MEM in) {
  bool zero_extend = in.ctrl.mem_addressing_ctrl.zero_extend;
  uint32_t addr_shift_amt = 32 - in.ctrl.mem_addressing_ctrl.addressing_width;

  MEM_WB out = {0};
  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }

  if (in.ctrl.data_mem_write_en) {
    data_mem[in.alu_res] = (in.data_i << addr_shift_amt) >> addr_shift_amt;
  }
  out.alu_res = in.alu_res;
  out.ctrl = in.ctrl;
  if (in.ctrl.data_mem_read_en) {
    uint32_t read_data = data_mem[in.alu_res];
    if (zero_extend) {
      out.data_o = (read_data << addr_shift_amt) >> addr_shift_amt;
    } else {
      out.data_o = ((int32_t)read_data << addr_shift_amt) >> addr_shift_amt;
    }
  }
  out.rd_addr = in.rd_addr;
  if (in.ctrl.data_mem_read_en || in.ctrl.data_mem_write_en)
    printf("Memory access - %s at address 0x%x, value: %d\n",
           in.ctrl.data_mem_write_en ? "Write" : "Read", in.alu_res,
           out.data_o);
  return out;
}
