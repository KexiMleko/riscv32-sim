#include "../alu.h"
#include "../pipe_regs.h"
#include <stdint.h>
#include <stdio.h>

EX_MEM execute(ID_EX in) {
  EX_MEM out = {0};

  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }
  uint32_t val2 = in.ctrl.alu_src_imm ? (uint32_t)in.imm : in.val2;

  out.alu_res = execute_alu(in.val1, val2, in.ctrl.alu_op);
  out.data_i = in.val2;
  out.ctrl = in.ctrl;
  out.rd_addr = in.rd_addr;

  printf("Execute - %x, result: %d \n", in.ctrl.alu_op,out.alu_res);
  return out;
}
