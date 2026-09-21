#include "store_buffers.h"
#include "memory/memory.h"
#include <stddef.h>
#include <stdio.h>
#include "trace.h"

#define SB_SLOT(tag) ((tag) - SB_TAG_BASE - 1)

static bool sb_tag_valid(uint32_t tag) {
  return tag > SB_TAG_BASE && tag <= SB_TAG_BASE + SB_CNT;
}

static store_buffer store_buffers[SB_CNT];

uint32_t sb_find_free_tag(void) {
  for (unsigned i = 0; i < SB_CNT; i++) {
    if (!store_buffers[i].busy) {
      return SB_TAG_BASE + i + 1;
    }
  }
  return 0;
}

bool sb_update(uint32_t tag, store_buffer sb_new) {
  if (!sb_tag_valid(tag)) {
    return false;
  }
  store_buffers[SB_SLOT(tag)] = sb_new;
  return true;
}

void sb_free(uint32_t tag) {
  if (!sb_tag_valid(tag)) {
    return;
  }
  store_buffers[SB_SLOT(tag)].busy = false;
}

uint32_t sb_find_ready(void) {
  for (unsigned i = 0; i < SB_CNT; i++) {
    if (store_buffers[i].busy) {
      return SB_TAG_BASE + i + 1;
    }
  }
  return 0;
}

void sb_commit(uint32_t tag, data_memory *dmem) {
  store_buffer *sb = &store_buffers[SB_SLOT(tag)];
  data_mem_write(sb->write_value, sb->address, sb->mem_ctrl.addressing_width,
                 dmem);
  trace_note(TR_EXEC, "[0x%04x] ← %d", sb->address, (int32_t)sb->write_value);
}

bool sb_all_empty(void) {
  for (size_t i = 0; i < SB_CNT; i++) {
    if (store_buffers[i].busy)
      return false;
  }
  return true;
}

void sb_render(char *buf, size_t cap) {
  buf[0] = '\0';
  for (size_t i = 0; i < SB_CNT; i++) {
    if (store_buffers[i].busy)
      trace_appendf(buf, cap, "%sSB%zu [0x%04x] ← %d", buf[0] ? "\n" : "",
                    i + 1, store_buffers[i].address,
                    (int32_t)store_buffers[i].write_value);
  }
}
