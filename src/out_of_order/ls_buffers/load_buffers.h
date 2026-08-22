#pragma once
#include "mem_addressing_ctrl.h"
#include "memory/memory.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool busy;
  uint32_t address;
  uint32_t destination_register;
  mem_addressing_ctrl_t mem_ctrl;
} load_buffer;

uint32_t lb_find_free_tag(void);
bool lb_add(uint32_t tag, uint32_t address, uint32_t rd,
            mem_addressing_ctrl_t mem_ctrl);
void lb_free(uint32_t tag);
uint32_t lb_execute(uint32_t tag, data_memory *dmem, uint32_t *rd_out);
bool lb_all_empty(void);
