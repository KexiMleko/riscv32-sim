#pragma once
#include "mem_addressing_ctrl.h"
#include "memory/memory.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool busy;
  uint32_t address;
  mem_addressing_ctrl_t mem_ctrl;
} load_buffer;

uint32_t lb_find_free_tag(void);
bool lb_update(uint32_t tag, load_buffer lb_new);
void lb_free(uint32_t tag);
uint32_t lb_find_ready(void);
uint32_t lb_execute(uint32_t tag, data_memory *dmem);
bool lb_all_empty(void);
