#include "store_buffers.h"
#include "memory/memory.h"
#include <stddef.h>
#include <stdio.h>

#define SB_CNT 4

static store_buffer store_buffers[SB_CNT];

uint32_t sb_find_free_tag(void) {
  for (unsigned i = 0; i < SB_CNT; i++) {
    if (!store_buffers[i].busy) {
      return i + 1;
    }
  }
  return 0;
}

bool sb_update(uint32_t tag, store_buffer sb_new) {
  if (tag == 0 || tag > SB_CNT) {
    return false;
  }
  store_buffers[tag - 1] = sb_new;
  return true;
}

void sb_free(uint32_t tag) {
  if (tag == 0 || tag > SB_CNT) {
    return;
  }
  store_buffers[tag - 1].busy = false;
}

uint32_t sb_find_ready(void) {
  for (unsigned i = 0; i < SB_CNT; i++) {
    if (store_buffers[i].busy) {
      return i + 1;
    }
  }
  return 0;
}

void sb_commit(uint32_t tag, data_memory *dmem) {
  store_buffer *sb = &store_buffers[tag - 1];
  data_mem_write(sb->write_value, sb->address, sb->mem_ctrl.addressing_width,
                 dmem);
  printf("[SB]  tag=%u addr=0x%08x val=%u committed\n", tag, sb->address,
         sb->write_value);
}

bool sb_all_empty(void) {
  for (size_t i = 0; i < SB_CNT; i++) {
    if (store_buffers[i].busy)
      return false;
  }
  return true;
}
