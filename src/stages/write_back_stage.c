#include "../pipe_regs.h"
#include "../regfile.h"
#include <stdint.h>
#include <stdio.h>

typedef bool halt_signal;
halt_signal write_back(MEM_WB in) {
  if (in.halt_signal) {
    return true;
  }
  if (in.ctrl.rd_we) {
    uint32_t wr_val = in.ctrl.mem_to_reg ? in.data_o : in.alu_res;
    rd_write(wr_val, in.rd_addr);
    printf("Write on x%d value: %d\n", in.rd_addr, wr_val);
  }
  return false;
}
