#include "branch_prediction/branch_prediction_unit.h"
#include "common/pipe_regs.h"
#include "instr_disasm.h"
#include "memory/memory.h"
#include "stages/pipeline.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
void ui_print_instr(uint32_t instr) {
  char instr_str[50];
  instr_disasm(instr, instr_str, 50);
  printf("Instruction fetched: %s\n", instr_str);
}
IF_ID instr_fetch(IF_ID in, instr_memory *instr_mem, uint32_t pc) {
  IF_ID out = {0};
  out.curr_pc = pc;

  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }

  branch_ctrl b_ctrl = in.b_ctrl;
  if (b_ctrl.pc_next_sel) {
    printf("Branching taken\n");
    pc = b_ctrl.next_pc;
    b_ctrl = (branch_ctrl){0};
  } else {
    pc = perform_prediction(pc);
    printf("Predicted %d\n",pc);
  }
  uint32_t instr = read_instr_mem(instr_mem, pc);
  if (instr == 0) {
    out.halt_signal = true;
    return out;
  }

  ui_print_instr(instr);

  pc += 4;
  printf("PC: %d\n", pc);
  out.instr = instr;
  out.pc = pc;
  return out;
}
