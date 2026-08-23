#include "alu.h"
#include "alu_op.h"
#include "common_data_bus/common_data_bus.h"
#include "control_decoder.h"
#include "imm_gen.h"
#include "instr_disasm.h"
#include "instr_fields.h"
#include "instruction_queue/instr_queue.h"
#include "ls_buffers/load_buffers.h"
#include "ls_buffers/store_buffers.h"
#include "ooo_pipeline.h"
#include "regfile/tomasulo_regfile.h"
#include "reservation_station/reservation_station.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

issue_result issue_instr(CDB cdb) {
  uint32_t instr = 0;
  issue_result res = {0};
  if (iq_front(&instr) != QUEUE_OK) {
    printf("[ISSUE] instruction queue empty\n");
    return res;
  }
  char instr_str[50];
  instr_disasm(instr, instr_str, 50);
  printf("[ISSUE] dequeued instr = %s\n", instr_str);
  uint32_t opcode = get_opcode(instr);
  uint32_t funct3 = get_funct3(instr);
  uint32_t funct7 = get_funct7(instr);
  uint32_t rs1 = get_rs1(instr);
  uint32_t rs2 = get_rs2(instr);
  uint32_t rd = get_rd(instr);

  control_signals ctrl = get_control_signals(opcode, funct3, funct7);
  int32_t imm = generate_imm(instr, ctrl.imm_type);

  // Computing effective address for load and store instructions
  if ((ctrl.data_mem_read_en || ctrl.data_mem_write_en) && ctrl.alu_src_imm) {
    if (regfile_read_tag(rs1)) {
      printf("[ISSUE] stall: no data available on REG[%d]\n", rs1);
      return res;
    }
    uint32_t addr = execute_alu(regfile_read_val(rs1), imm, ALU_ADD);
    if (ctrl.data_mem_read_en) {
      uint32_t lb_free_tag = lb_find_free_tag();
      if (!lb_free_tag) {
        printf("[ISSUE] stall: no free load buffer\n");
        return res;
      }
      load_buffer lb = (load_buffer){
          .address = addr, .busy = true, .mem_ctrl = ctrl.mem_addressing_ctrl};
      res = (issue_result){
          .ctrl = ctrl, .lb = lb, .rd = rd, .tag = lb_free_tag, .valid = true};
      return res;
    }
    if (ctrl.data_mem_write_en) {
      uint32_t sb_free_tag = sb_find_free_tag();
      if (!sb_free_tag) {
        printf("[ISSUE] stall: no free store buffer\n");
        return res;
      }
      store_buffer sb = (store_buffer){.address = addr,
                                       .busy = true,
                                       .mem_ctrl = ctrl.mem_addressing_ctrl,
                                       .write_value = rd};
      res = (issue_result){
          .ctrl = ctrl, .sb = sb, .tag = sb_free_tag, .valid = true};
      return res;
    }
  }

  uint32_t rs_free_tag = rs_find_free_tag();
  if (!rs_free_tag) {
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

  if (cdb.valid) {
    if (rs.rs1_tag == cdb.tag) {
      rs.rs1_tag = 0;
      rs.rs1_val = cdb.value;
    }
    if (rs.rs2_tag == cdb.tag) {
      rs.rs2_tag = 0;
      rs.rs2_val = cdb.value;
    }
  }
  rs.busy = true;
  res.valid = true;
  res.rs = rs;
  res.rd = rd;
  res.tag = rs_free_tag;

  printf("[ISSUE] -> RS[%u] op=0x%02x rd=x%u rs1_tag=%u rs1_val=%d "
         "rs2_tag=%u rs2_val=%d\n",
         rs_free_tag, rs.op, rd, rs.rs1_tag, rs.rs1_val, rs.rs2_tag,
         rs.rs2_val);
  return res;
}
