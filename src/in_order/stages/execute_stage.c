#include "common/alu.h"
#include "common/pipe_regs.h"
#include "forwarding/forwarding_unit.h"
#include "stages/pipeline.h"
#include <stdint.h>
#include "trace.h"
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
    trace_note(TR_EX, "fwd MEM→EX x%u", in.ctrl.rs1_addr);
    break;
  case FW_WB_RES:
    in.val1 = fw_data.alu_res_wb;
    trace_note(TR_EX, "fwd WB→EX x%u", in.ctrl.rs1_addr);
    break;
  }
  switch (fw_sigs.alu_forward_b) {
  case FW_NO_DEPS:
    break;
  case FW_MEM_RES:
    in.val2 = fw_data.alu_res_mem;
    trace_note(TR_EX, "fwd MEM→EX x%u", in.ctrl.rs2_addr);
    break;
  case FW_WB_RES:
    in.val2 = fw_data.alu_res_wb;
    trace_note(TR_EX, "fwd WB→EX x%u", in.ctrl.rs2_addr);
    break;
  }

  uint32_t val2 = in.ctrl.alu_src_imm ? (uint32_t)in.imm : in.val2;

  out.alu_res = execute_alu(in.val1, val2, in.ctrl.alu_op);
  out.data_i = in.val2;
  out.ctrl = in.ctrl;
  out.rd_addr = in.rd_addr;
  out.instr = in.instr;

  if (in.ctrl.data_mem_read_en || in.ctrl.data_mem_write_en)
    trace_note(TR_EX, "addr 0x%04x", out.alu_res);
  else if (in.ctrl.rd_we)
    trace_note(TR_EX, "→ %d", (int32_t)out.alu_res);
  return out;
}
