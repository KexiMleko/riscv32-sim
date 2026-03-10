#include "common/alu.h"
#include "common/pipe_regs.h"
#include "forwarding/forwarding_unit.h"
#include "stages/pipeline.h"
#include <stdint.h>
// #include <stdio.h>
EX_MEM execute(ID_EX in, struct fw_data fw_data) {
  EX_MEM out = {0};

  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }
  struct fw_signals fw_sigs = fw_data.fw_signals;
  switch (fw_sigs.alu_forward_a) {
  case FW_NO_DEPS:
    break;
  case FW_MEM_RES:
    in.val1 = fw_data.alu_res_mem;
    break;
  case FW_WB_RES:
    in.val1 = fw_data.alu_res_wb;
    break;
  }
  switch (fw_sigs.alu_forward_b) {
  case FW_NO_DEPS:
    break;
  case FW_MEM_RES:
    in.val2 = fw_data.alu_res_mem;
    break;
  case FW_WB_RES:
    in.val2 = fw_data.alu_res_wb;
    break;
  }

  uint32_t val2 = in.ctrl.alu_src_imm ? (uint32_t)in.imm : in.val2;

  out.alu_res = execute_alu(in.val1, val2, in.ctrl.alu_op);
  out.data_i = in.val2;
  out.ctrl = in.ctrl;
  out.rd_addr = in.rd_addr;

  // printf("Execute - %x, result: %d \n", in.ctrl.alu_op, out.alu_res);
  return out;
}
