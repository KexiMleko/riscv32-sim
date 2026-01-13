#include "../pipe_regs.h"
#include <stdio.h>

extern uint32_t data_mem[1024];
MEM_WB memory_access(EX_MEM in) {
  MEM_WB out = {0};
  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }
  if (in.ctrl.data_mem_we) {
    data_mem[in.alu_res] = in.data_i;
  }
  out.alu_res = in.alu_res;
  out.ctrl = in.ctrl;
  out.data_o = data_mem[in.alu_res];
  out.rd_addr = in.rd_addr;
  printf("Memory access - %s\n", in.ctrl.data_mem_we ? "Write" : "Read");
  return out;
}
