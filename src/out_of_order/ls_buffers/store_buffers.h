#pragma once
#include "mem_addressing_ctrl.h"
#include "memory/memory.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool busy;
  uint32_t address;
  uint32_t write_value;
  mem_addressing_ctrl_t mem_ctrl;
} store_buffer;

uint32_t sb_find_free_tag(void);
bool sb_update(uint32_t tag, store_buffer sb_new);
void sb_free(uint32_t tag);
uint32_t sb_find_ready(void);
void sb_commit(uint32_t tag, data_memory *dmem);
bool sb_all_empty(void);
