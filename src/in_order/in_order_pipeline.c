#include "config/params.h"
#include "memory/memory.h"
#include "pipe_regs.h"
#include "pipeline.h"
#include "pipeline_interface.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool run_inorder_pipeline(int32_t PC, instr_memory *instr_mem,
                          data_memory *data_mem) {

  uint64_t clk_cycle = 0;

  IF_ID if_id_reg = {0};
  ID_EX id_ex_reg = {0};
  EX_MEM ex_mem_reg = {0};
  MEM_WB mem_wb_reg = {0};
  branch_ctrl b_ctrl = {.next_pc = 0, .pc_next_sel = false};
  halt_signal halt = false;

  printf("\nPress Enter to cycle...\n");
  while (!halt) {
    clk_cycle++;
    if (!mem_wb_reg.halt_signal) {
      fflush(stdout);
      (void)getchar();
    }

    if (clk_cycle >= MAX_CLK_COUNT) {
      printf("\nMax clock cycle count reached: Stopping simulation\n");
      return EXIT_FAILURE;
    }
    halt = write_back(mem_wb_reg);
    mem_wb_reg = memory_access(ex_mem_reg, data_mem);
    ex_mem_reg = execute(id_ex_reg);

    id_ex_reg = instr_decode(if_id_reg, &b_ctrl);
    if_id_reg = instr_fetch(if_id_reg, instr_mem, PC);
    /*
     * if_id_reg.b_ctrl = b_ctrl is here to provide the illusion of instr_decode
     * branch evaluation in cycle t and instr_fetch seeing that change through a
     * register in cycle t+1.
     */
    if_id_reg.b_ctrl = b_ctrl;

    PC = if_id_reg.pc;
  }
  return EXIT_SUCCESS;
}
