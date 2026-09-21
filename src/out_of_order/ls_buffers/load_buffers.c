#include "load_buffers.h"
#include "memory/memory.h"
#include <stddef.h>
#include <stdio.h>
#include "trace.h"

#define LB_SLOT(tag) ((tag) - LB_TAG_BASE - 1)

static bool lb_tag_valid(uint32_t tag) {
  return tag > LB_TAG_BASE && tag <= LB_TAG_BASE + LB_CNT;
}

static load_buffer load_buffers[LB_CNT];

uint32_t lb_find_free_tag(void) {
  for (unsigned i = 0; i < LB_CNT; i++) {
    if (!load_buffers[i].busy) {
      return LB_TAG_BASE + i + 1;
    }
  }
  return 0;
}

bool lb_update(uint32_t tag, load_buffer lb_new) {
  if (!lb_tag_valid(tag)) {
    return false;
  }
  load_buffers[LB_SLOT(tag)] = lb_new;
  return true;
}

void lb_free(uint32_t tag) {
  if (!lb_tag_valid(tag)) {
    return;
  }
  load_buffers[LB_SLOT(tag)].busy = false;
}

uint32_t lb_find_ready(void) {
  for (unsigned i = 0; i < LB_CNT; i++) {
    if (load_buffers[i].busy) {
      return LB_TAG_BASE + i + 1;
    }
  }
  return 0;
}

uint32_t lb_execute(uint32_t tag, data_memory *dmem) {
  load_buffer *lb = &load_buffers[LB_SLOT(tag)];
  uint32_t val = data_mem_read(lb->address, lb->mem_ctrl.zero_extend,
                               lb->mem_ctrl.addressing_width, dmem);
  trace_note(TR_EXEC, "[0x%04x] → %d", lb->address, (int32_t)val);
  return val;
}

bool lb_all_empty(void) {
  for (size_t i = 0; i < LB_CNT; i++) {
    if (load_buffers[i].busy)
      return false;
  }
  return true;
}

void lb_render(char *buf, size_t cap) {
  buf[0] = '\0';
  for (size_t i = 0; i < LB_CNT; i++) {
    if (load_buffers[i].busy)
      trace_appendf(buf, cap, "%sLB%zu [0x%04x]", buf[0] ? "\n" : "", i + 1,
                    load_buffers[i].address);
  }
}
