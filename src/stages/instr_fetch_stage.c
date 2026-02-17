#include "../pipe_regs.h"
#include "pipeline.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern uint32_t instr_mem[1024];
IF_ID instr_fetch(IF_ID in, branch_ctrl *b_ctrl, uint32_t pc) {
  IF_ID out = {0};
  out.curr_pc=pc;

  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }
  uint32_t instr = instr_mem[pc >> 2];
  if (instr == 0) {
    out.halt_signal = true;
    return out;
  }
  printf("Instruction fetched: 0x%08x at %d\n",instr,pc);
  if (b_ctrl->pc_next_sel) {
    printf("Branching taken\n");
    pc = b_ctrl->next_pc;
    instr = instr_mem[pc >> 2];
    b_ctrl->pc_next_sel = false;
    b_ctrl->next_pc = 0;
  } 
   pc += 4;
  printf("PC: %d\n", pc);
  out.instr = instr;
  out.pc = pc;
  return out;
}
