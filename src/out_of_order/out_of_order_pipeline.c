
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
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// TOMASULO
bool run_ooo_pipeline(int32_t PC, instr_memory *instr_mem,
                      data_memory *data_mem) {

  CDB cdb = {0};

  uint64_t clk_cycle = 0;
  halt_signal halt = false;
  bool halt_pending = false;

  IF_ID if_id_reg = {0};
  branch_ctrl b_ctrl = {.next_pc = 0, .pc_next_sel = false};
  printf("\nPress Enter to cycle...\n");

  while (!halt) {
    fflush(stdout);
    (void)getchar();

    clk_cycle++;

    if (clk_cycle >= MAX_CLK_COUNT) {
      printf("\nMax clock cycle count reached: Stopping simulation\n");
      return EXIT_FAILURE;
    }
    IF_ID if_id_next = ooo_instr_fetch(if_id_reg, instr_mem, PC);
    issue_result issue_res = issue_instr(cdb);
    exec_result exec_res = ooo_execute(data_mem);
    halt_signal halt_temp = write_result(cdb, halt_pending);

    // SEQUENTIAL
    if_id_reg = if_id_next;
    printf("%u\n", if_id_reg.instr);
    if (!if_id_reg.halt_signal)
      enqueue_instr(if_id_reg.instr);
    if (issue_res.valid) {
      printf("\nISSUE RES VALID\n");
      if (issue_res.ctrl.data_mem_read_en) {
        lb_update(issue_res.tag, issue_res.lb);
        regfile_update_tag(issue_res.rd, issue_res.tag);
      } else if (issue_res.ctrl.data_mem_write_en) {
        sb_update(issue_res.tag, issue_res.sb);
      } else {
        regfile_update_tag(issue_res.rd, issue_res.tag);
        rs_update(issue_res.tag, issue_res.rs);
      }
      dequeue_instr();
    }
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
      printf("\nHALT PENDING\n");
    halt = halt_temp;
  }
  return EXIT_SUCCESS;
}
