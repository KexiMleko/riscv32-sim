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

bool sb_add(uint32_t tag, uint32_t address, uint32_t write_value,
            mem_addressing_ctrl_t mem_ctrl) {
  if (tag == 0 || tag > SB_CNT) {
    return false;
  }
  store_buffers[tag - 1] = (store_buffer){
      .busy = true,
      .address = address,
      .write_value = write_value,
      .mem_ctrl = mem_ctrl,
  };
  return true;
}

void sb_free(uint32_t tag) {
  if (tag == 0 || tag > SB_CNT) {
    return;
  }
  store_buffers[tag - 1].busy = false;
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
