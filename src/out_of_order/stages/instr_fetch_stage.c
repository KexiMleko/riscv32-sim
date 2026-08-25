#include "branch_prediction/branch_prediction_unit.h"
#include "common/pipe_regs.h"
#include "instr_disasm.h"
#include "memory/memory.h"
#include "stages/ooo_pipeline.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static void ui_print_instr(uint32_t instr) {
  char instr_str[50];
  instr_disasm(instr, instr_str, 50);
  printf("Instruction fetched: %s\n", instr_str);
}
IF_ID ooo_instr_fetch(IF_ID in, instr_memory *instr_mem, uint32_t pc) {
  IF_ID out = {0};
  out.curr_pc = pc;

  if (branch_pending) {
    printf("[FETCH] stall: branch_pending, emitting bubble\n");
    out.instr = 0;
    out.pc = pc;
    return out;
  }
  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }

  if (in.b_ctrl.pc_next_sel) {
    pc = in.b_ctrl.next_pc;
    printf("Branching taken pc = %d\n",pc);
  } else {
    pc = perform_prediction(pc);
    printf("Predicted %d\n", pc);
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
