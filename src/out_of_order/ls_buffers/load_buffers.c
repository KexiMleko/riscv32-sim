#include "load_buffers.h"
#include "memory/memory.h"
#include <stddef.h>
#include <stdio.h>

#define LB_CNT 4

static load_buffer load_buffers[LB_CNT];

uint32_t lb_find_free_tag(void) {
  for (unsigned i = 0; i < LB_CNT; i++) {
    if (!load_buffers[i].busy) {
      return i + 1;
    }
  }
  return 0;
}

bool lb_update(uint32_t tag, load_buffer lb_new) {
  if (tag == 0 || tag > LB_CNT) {
    return false;
  }
  load_buffers[tag - 1] = lb_new;
  return true;
}

void lb_free(uint32_t tag) {
  if (tag == 0 || tag > LB_CNT) {
    return;
  }
  load_buffers[tag - 1].busy = false;
}

uint32_t lb_find_ready(void) {
  for (unsigned i = 0; i < LB_CNT; i++) {
    if (load_buffers[i].busy) {
      return i + 1;
    }
  }
  return 0;
}

uint32_t lb_execute(uint32_t tag, data_memory *dmem) {
  load_buffer *lb = &load_buffers[tag - 1];
  uint32_t val = data_mem_read(lb->address, lb->mem_ctrl.zero_extend,
                               lb->mem_ctrl.addressing_width, dmem);
  printf("[LB]  tag=%u addr=0x%08x -> val=%d\n", tag, lb->address, val);
  return val;
}

bool lb_all_empty(void) {
  for (size_t i = 0; i < LB_CNT; i++) {
    if (load_buffers[i].busy)
      return false;
  }
  return true;
}
