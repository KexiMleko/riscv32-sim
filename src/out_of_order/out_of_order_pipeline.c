
#include "common_data_bus/common_data_bus.h"
#include "config/params.h"
#include "halt_signal.h"
#include "instruction_queue/instr_queue.h"
#include "ls_buffers/load_buffers.h"
#include "ls_buffers/store_buffers.h"
#include "memory/memory.h"
#include "out_of_order/stages/ooo_pipeline.h"
#include "pipe_regs.h"
#include "regfile/tomasulo_regfile.h"
#include "reservation_station/reservation_station.h"
#include "trace.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Renders IQ/RS/LB/SB/register state, sections are only printed when changed
static void trace_ooo_state(void) {
  char buf[512];
  iq_render(buf, sizeof(buf));
  trace_section("IQ", buf);
  rs_render(buf, sizeof(buf));
  trace_section("RS", buf);
  lb_render(buf, sizeof(buf));
  trace_section("LB", buf);
  sb_render(buf, sizeof(buf));
  trace_section("SB", buf);
  tomasulo_regfile_render(buf, sizeof(buf));
  trace_section("REGS", buf);
}

// TOMASULO
bool run_ooo_pipeline(int32_t PC, instr_memory *instr_mem,
                      data_memory *data_mem) {

  CDB cdb = {0};

  uint64_t clk_cycle = 0;
  halt_signal halt = false;
  bool halt_pending = false;

  IF_ID if_id_reg = {0};
  trace_init(TRACE_OOO);
  printf("\nPress Enter to cycle...\n");

  while (!halt) {
    fflush(stdout);
    (void)getchar();

    clk_cycle++;

    if (clk_cycle >= MAX_CLK_COUNT) {
      printf("\nMax clock cycle count reached: Stopping simulation\n");
      return EXIT_FAILURE;
    }
    trace_begin_cycle(clk_cycle, if_id_reg.halt_signal ? TRACE_NO_PC
                                 : if_id_reg.b_ctrl.pc_next_sel
                                     ? if_id_reg.b_ctrl.next_pc
                                     : (uint32_t)PC);
    IF_ID if_id_next = ooo_instr_fetch(if_id_reg, instr_mem, PC);
    issue_result issue_res = issue_instr(cdb);
    exec_result exec_res = ooo_execute(data_mem);
    halt_signal halt_temp = write_result(cdb, halt_pending);

    // SEQUENTIAL
    if_id_reg = if_id_next;
    if (!if_id_reg.halt_signal && if_id_reg.instr != 0)
      enqueue_instr(if_id_reg.instr, if_id_reg.curr_pc);
    if (issue_res.valid) {
      if_id_reg.b_ctrl = issue_res.b_ctrl;
      if (issue_res.ctrl.data_mem_read_en) {
        lb_update(issue_res.tag, issue_res.lb);
        regfile_update_tag(issue_res.rd, issue_res.tag);
      } else if (issue_res.ctrl.data_mem_write_en) {
        sb_update(issue_res.tag, issue_res.sb);
      } else if (issue_res.ctrl.branch) {
        trace_stat_retire();
        if (if_id_reg.b_ctrl.pc_next_sel) {
          trace_note(TR_ISSUE, "redirect → 0x%04x, flush IQ",
                     if_id_reg.b_ctrl.next_pc);
          trace_stat_flush();
          iq_flush();
        }
      } else {
        regfile_update_tag(issue_res.rd, issue_res.tag);
        rs_update(issue_res.tag, issue_res.rs);
      }
      dequeue_instr();
    }
    if (exec_res.freed_unit != UNIT_NONE)
      trace_stat_retire();
    switch (exec_res.freed_unit) {
    case UNIT_RS:
      rs_free(exec_res.freed_tag);
      break;
    case UNIT_LB:
      lb_free(exec_res.freed_tag);
      break;
    case UNIT_SB:
      sb_free(exec_res.freed_tag);
      break;
    case UNIT_NONE:
      break;
    }

    PC = if_id_reg.pc;
    if (exec_res.cdb.valid)
      cdb = exec_res.cdb;
    halt_pending = if_id_reg.halt_signal;
    if (halt_pending)
      trace_note(TR_FETCH, "end of program, draining");
    halt = halt_temp;
    trace_ooo_state();
    trace_end_cycle();
  }

  char regs[512], mem[512];
  tomasulo_regfile_render(regs, sizeof(regs));
  data_mem_render(data_mem, mem, sizeof(mem));
  trace_summary(clk_cycle, regs, mem);
  return EXIT_SUCCESS;
}
