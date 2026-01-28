#include "../branch_ctrl.h"
#include "../control_decoder.h"
#include "../imm_gen.h"
#include "../instr_fields.h"
#include "../pipe_regs.h"
#include "../regfile.h"
#include "pipeline.h"
#include <stdint.h>
#include <stdio.h>

ID_EX instr_decode(IF_ID in) {
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

  uint32_t next_pc = 0;
  if (ctrl.branch && eval_branch(rs1, rs2, get_funct3(instr))) {
    next_pc = in.pc + imm;
    printf("branching taken, pc is %d\n", next_pc);
  }
  out.imm = imm;
  out.rd_addr = rd;
  out.ctrl = ctrl;
  out.val1 = rb_out.rs1_data;
  out.val2 = rb_out.rs2_data;
  printf("Instruction decoded - %d\n", instr);

  return out;
}
