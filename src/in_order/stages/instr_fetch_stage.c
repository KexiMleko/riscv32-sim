#include "branch_prediction/branch_prediction_unit.h"
#include "common/pipe_regs.h"
#include "trace.h"
#include "memory/memory.h"
#include "stages/pipeline.h"
#include <stdbool.h>
#include <stdint.h>
IF_ID instr_fetch(IF_ID in, instr_memory *instr_mem, uint32_t pc) {
  IF_ID out = {0};
  out.curr_pc = pc;

  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }

  if (in.b_ctrl.pc_next_sel) {
    pc = in.b_ctrl.next_pc;
    trace_note(TR_IF, "redirect → 0x%04x", pc);
  } else {
    uint32_t predicted = perform_prediction(pc);
    if (predicted != pc)
      trace_note(TR_IF, "BTB predicts taken → 0x%04x", predicted);
    pc = predicted;
  }
  uint32_t instr = read_instr_mem(instr_mem, pc);
  if (instr == 0) {
    out.halt_signal = true;
    trace_note(TR_IF, "end of program");
    return out;
  }

  pc += 4;
  out.instr = instr;
  out.pc = pc;
  return out;
}
