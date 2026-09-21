#include "memory/memory.h"
#include "common/mem_addressing_ctrl.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "trace.h"

uint32_t read_instr_mem(instr_memory *instr_mem, uint32_t pc) {
  return instr_mem->words[pc >> 2];
}

uint32_t data_mem_read(uint32_t addr, bool zero_extend, mem_width_t width,
                       data_memory *data_mem) {
  uint32_t shift_amt = 32 - width;
  uint32_t read_data = data_mem->words[addr >> 2];
  uint32_t out;
  if (zero_extend) {
    out = (read_data << shift_amt) >> shift_amt;
  } else {
    out = ((int32_t)read_data << shift_amt) >> shift_amt;
  }
  return out;
}

void data_mem_write(uint32_t w_val, uint32_t dest_addr, mem_width_t width,
                    data_memory *data_mem) {
  uint32_t shift_amt = 32 - width;
  if (data_mem->size <= dest_addr) {
    fprintf(stderr, "Out-of-Bounds Memory Access\nNOTE: You can increase data memory size "
                    "using --dmem-size=[size]\n");
    exit(EXIT_FAILURE);
  }
  data_mem->words[dest_addr >> 2] = (w_val << shift_amt) >> shift_amt;
}

#define MEM_RENDER_MAX_ENTRIES 16

void data_mem_render(const data_memory *data_mem, char *buf, size_t cap) {
  buf[0] = '\0';
  unsigned shown = 0;
  for (uint32_t i = 0; i < data_mem->size; i++) {
    if (data_mem->words[i] == 0)
      continue;
    if (shown == MEM_RENDER_MAX_ENTRIES) {
      trace_appendf(buf, cap, "  ...");
      return;
    }
    trace_appendf(buf, cap, "%s[0x%04x]=%d", shown ? "  " : "", i << 2,
                  (int32_t)data_mem->words[i]);
    shown++;
  }
}
