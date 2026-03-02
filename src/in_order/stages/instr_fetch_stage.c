#include "common/pipe_regs.h"
#include "memory/memory.h"
#include "stages/pipeline.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

IF_ID instr_fetch(IF_ID in, instr_memory *instr_mem, uint32_t pc) {
  IF_ID out = {0};
  out.curr_pc = pc;

  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }
  uint32_t instr = read_instr_mem(instr_mem, pc);
  if (instr == 0) {
    out.halt_signal = true;
    return out;
  }
  printf("Instruction fetched: 0x%08x at %d\n", instr, pc);
  branch_ctrl b_ctrl = in.b_ctrl;
  if (b_ctrl.pc_next_sel) {
    printf("Branching taken\n");
    pc = b_ctrl.next_pc;
    instr = read_instr_mem(instr_mem, pc);
    b_ctrl.pc_next_sel = false;
    b_ctrl.next_pc = 0;
  }
  pc += 4;
  printf("PC: %d\n", pc);
  out.instr = instr;
  out.pc = pc;
  return out;
}
