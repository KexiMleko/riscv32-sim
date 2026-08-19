
#include "config/params.h"
#include "halt_signal.h"
#include "instruction_queue/instr_queue.h"
#include "memory/memory.h"
#include "out_of_order/stages/ooo_pipeline.h"
#include "pipe_regs.h"
#include "reservation_station/reservation_station.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define RS_CNT 8

// TOMASULO
bool run_ooo_pipeline(int32_t PC, instr_memory *instr_mem,
                      data_memory *data_mem) {

  reservation_station reservation_stations[RS_CNT] = {0};

  uint64_t clk_cycle = 0;
  halt_signal halt = false;

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
    IF_ID if_id_next = instr_fetch(if_id_reg, instr_mem, PC);
    issue_instr(reservation_stations, RS_CNT);
    // SEQUENTIAL
    if_id_reg = if_id_next;
    enqueue_instr(if_id_reg.instr);
  }
  return EXIT_SUCCESS;
}
