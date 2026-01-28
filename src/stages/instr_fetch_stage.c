#include "../pipe_regs.h"
#include "pipeline.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern uint32_t instr_mem[1024];
IF_ID instr_fetch(IF_ID in, uint32_t pc) {
  IF_ID out = {0};
  if (in.halt_signal) {
    out.halt_signal = true;
    return out;
  }
  uint32_t instr = instr_mem[pc >> 2];
  if (instr == 0) {
    out.halt_signal = true;
    return out;
  }
  printf("Instruction fetched: %08x at PC: %d\n", instr, pc);
  if (in.pc_next_sel) {
    pc = in.next_pc;
  } else {
    pc += 4;
  }
  out.instr = instr;
  out.pc = pc;
  return out;
}
