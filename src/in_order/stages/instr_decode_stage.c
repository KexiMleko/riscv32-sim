#include "branch_prediction/branch_prediction_unit.h"
#include "common/branch_ctrl.h"
#include "common/control_decoder.h"
#include "common/imm_gen.h"
#include "common/instr_fields.h"
#include "common/pipe_regs.h"
#include "common/regfile.h"
#include "stages/pipeline.h"
#include <stdint.h>
#include <stdio.h>

ID_EX instr_decode(IF_ID in, branch_ctrl *b_ctrl, bool *b_flush) {
  ID_EX out = {0};
  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }
  uint32_t instr = in.instr;
  uint32_t opcode = get_opcode(instr);
  uint32_t funct3 = get_funct3(instr);
  uint32_t funct7 = get_funct7(instr);
  uint32_t rs1 = get_rs1(instr);
  uint32_t rs2 = get_rs2(instr);
  uint32_t rd = get_rd(instr);
  control_signals ctrl = get_control_signals(opcode, funct3, funct7);

  regfile_out rb_out = regfile_read(rs1, rs2);
  int32_t imm = generate_imm(instr, ctrl.imm_type);

  if (ctrl.branch) {
    uint32_t target = in.curr_pc + imm;
    bool taken =
        eval_branch(rb_out.rs1_data, rb_out.rs2_data, get_funct3(instr));
    printf("flush eval - param 1: %d, param 2: %d\n", in.pc,
           (taken ? target : in.curr_pc + 4));
    if (in.pc != (taken ? target : in.curr_pc + 4)) {
      printf("IFID FLUSH!\n");
      *b_flush = true;
    }
    b_ctrl->next_pc = target;
    b_ctrl->pc_next_sel = taken;
    printf("Branch evaluated - %d cmp %d - %s\n", rb_out.rs1_data,
           rb_out.rs2_data, taken ? "Taken" : "Not taken");

    record_branch_result(in.curr_pc, target, taken);

  } else {
    b_ctrl->pc_next_sel = false;
  }

  out.imm = imm;
  out.rd_addr = rd;
  out.ctrl = ctrl;
  out.val1 = rb_out.rs1_data;
  out.val2 = rb_out.rs2_data;

  return out;
}
