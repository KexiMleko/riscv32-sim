#include "control_decoder.h"
#include "imm_gen.h"
#include "instr_fields.h"
#include "instruction_queue/instr_queue.h"
#include "ooo_pipeline.h"
#include "regfile/tomasulo_regfile.h"
#include "reservation_station/reservation_station.h"
#include <stdint.h>
#include <stdio.h>

issue_result issue_instr() {
  uint32_t instr = 0;
  issue_result res = {0};
  if (iq_front(&instr) != QUEUE_OK) {
    printf("[ISSUE] instruction queue empty\n");
    return res;
  }
  printf("[ISSUE] dequeued instr=0x%08x\n", instr);
  uint32_t opcode = get_opcode(instr);
  uint32_t funct3 = get_funct3(instr);
  uint32_t funct7 = get_funct7(instr);
  uint32_t rs1 = get_rs1(instr);
  uint32_t rs2 = get_rs2(instr);
  uint32_t rd = get_rd(instr);

  control_signals ctrl = get_control_signals(opcode, funct3, funct7);

  int32_t imm = generate_imm(instr, ctrl.imm_type);

  uint32_t ready_tag = rs_find_free_tag();
  if (!ready_tag) {
    printf("[ISSUE] stall: no free reservation station\n");
    return res;
  }
  reservation_station rs = {0};

  rs.op = ctrl.alu_op;

  if (regfile_read_tag(rs1) == 0) {
    rs.rs1_val = regfile_read_val(rs1);
  } else {
    rs.rs1_tag = regfile_read_tag(rs1);
  }
  if (ctrl.alu_src_imm) {
    rs.rs2_val = (imm);
  } else {
    if (regfile_read_tag(rs2) == 0) {
      rs.rs2_val = regfile_read_val(rs2);
    } else {
      rs.rs2_tag = regfile_read_tag(rs2);
    }
  }
  rs.busy = true;
  res.valid = true;
  res.rs = rs;
  res.rd = rd;
  res.tag = ready_tag;

  printf("[ISSUE] -> RS[%u] op=0x%02x rd=x%u rs1_tag=%u rs1_val=%d "
         "rs2_tag=%u rs2_val=%d\n",
         ready_tag, rs.op, rd, rs.rs1_tag, rs.rs1_val, rs.rs2_tag, rs.rs2_val);
  return res;
}
