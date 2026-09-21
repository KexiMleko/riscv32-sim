#include "alu.h"
#include "alu_op.h"
#include "branch_ctrl.h"
#include "common_data_bus/common_data_bus.h"
#include "control_decoder.h"
#include "imm_gen.h"
#include "instr_fields.h"
#include "instruction_queue/instr_queue.h"
#include "ls_buffers/load_buffers.h"
#include "ls_buffers/store_buffers.h"
#include "ooo_pipeline.h"
#include "regfile/tomasulo_regfile.h"
#include "reservation_station/reservation_station.h"
#include <stdbool.h>
#include <stdint.h>
#include "trace.h"

bool branch_pending = false;

issue_result issue_instr(CDB cdb) {
  iq_entry entry = {0};
  issue_result res = {0};
  if (iq_front(&entry) != QUEUE_OK) {
    return res;
  }
  uint32_t instr = entry.instr;
  uint32_t pc = entry.pc;
  trace_row_instr(TR_ISSUE, instr);
  uint32_t opcode = get_opcode(instr);
  uint32_t funct3 = get_funct3(instr);
  uint32_t funct7 = get_funct7(instr);
  uint32_t rs1 = get_rs1(instr);
  uint32_t rs2 = get_rs2(instr);
  uint32_t rd = get_rd(instr);

  control_signals ctrl = get_control_signals(opcode, funct3, funct7);
  int32_t imm = generate_imm(instr, ctrl.imm_type);

  if (ctrl.branch) {
    uint32_t t1 = regfile_read_tag(rs1);
    uint32_t t2 = regfile_read_tag(rs2);
    uint32_t v1 = regfile_read_val(rs1);
    uint32_t v2 = regfile_read_val(rs2);

    // Snoop CDB so we can resolve the same cycle the operand arrives
    if (cdb.valid && t1 != 0 && t1 == cdb.tag) {
      t1 = 0;
      v1 = cdb.value;
    }
    if (cdb.valid && t2 != 0 && t2 == cdb.tag) {
      t2 = 0;
      v2 = cdb.value;
    }

    if (t1 || t2) {
      trace_note(TR_ISSUE, "STALL: branch waits on #%u", t1 ? t1 : t2);
      trace_stat_stall("branch operand");
      branch_pending = true; // keep fetch stalled, retry next cycle
      return res;            // res.valid still false -> no dequeue
    }

    bool taken = eval_branch(v1, v2, funct3);
    uint32_t target = pc + imm;

    branch_pending = false;

    trace_note(TR_ISSUE, "branch %s (%d vs %d)", taken ? "taken" : "not taken",
               (int32_t)v1, (int32_t)v2);

    res.b_ctrl = (branch_ctrl){.next_pc = target, .pc_next_sel = taken};
    res.valid = true;
    res.ctrl = ctrl;
    return res;
  }
  // Computing effective address for load and store instructions
  if ((ctrl.data_mem_read_en || ctrl.data_mem_write_en) && ctrl.alu_src_imm) {
    if (regfile_read_tag(rs1)) {
      trace_note(TR_ISSUE, "STALL: base x%u pending", rs1);
      trace_stat_stall("base reg pending");
      return res;
    }
    uint32_t addr = execute_alu(regfile_read_val(rs1), imm, ALU_ADD);
    if (ctrl.data_mem_read_en) {
      uint32_t lb_free_tag = lb_find_free_tag();
      if (!lb_free_tag) {
        trace_note(TR_ISSUE, "STALL: no free LB");
        trace_stat_stall("no free LB");
        return res;
      }
      load_buffer lb = (load_buffer){
          .address = addr, .busy = true, .mem_ctrl = ctrl.mem_addressing_ctrl};
      res = (issue_result){
          .ctrl = ctrl, .lb = lb, .rd = rd, .tag = lb_free_tag, .valid = true};
      trace_note(TR_ISSUE, "→ LB%u", lb_free_tag - LB_TAG_BASE);
      return res;
    }
    if (ctrl.data_mem_write_en) {
      uint32_t sb_free_tag = sb_find_free_tag();
      if (!sb_free_tag) {
        trace_note(TR_ISSUE, "STALL: no free SB");
        trace_stat_stall("no free SB");
        return res;
      }
      // Store data comes from rs2. If it is still in flight the SB waits for
      // it on the CDB (rs2 is not the rd field, S-type has no rd)
      uint32_t data_tag = regfile_read_tag(rs2);
      uint32_t data_val = regfile_read_val(rs2);
      if (cdb.valid && data_tag != 0 && data_tag == cdb.tag) {
        data_tag = 0;
        data_val = cdb.value;
      }
      store_buffer sb = (store_buffer){.address = addr,
                                       .busy = true,
                                       .mem_ctrl = ctrl.mem_addressing_ctrl,
                                       .write_value = data_val,
                                       .value_tag = data_tag};
      res = (issue_result){
          .ctrl = ctrl, .sb = sb, .tag = sb_free_tag, .valid = true};
      trace_note(TR_ISSUE, "→ SB%u", sb_free_tag - SB_TAG_BASE);
      if (data_tag)
        trace_note(TR_ISSUE, "data waits on #%u", data_tag);
      return res;
    }
  }

  uint32_t rs_free_tag = rs_find_free_tag();
  if (!rs_free_tag) {
    trace_note(TR_ISSUE, "STALL: no free RS");
    trace_stat_stall("no free RS");
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

  trace_note(TR_ISSUE, "→ RS%u", rs_free_tag);
  return res;
}
