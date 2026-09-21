#pragma once
#include "mem_addressing_ctrl.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint32_t size;
  uint32_t words[];
} instr_memory;

typedef struct {
  uint32_t size;
  uint32_t words[];
} data_memory;

uint32_t read_instr_mem(instr_memory *instr_mem, uint32_t pc);

void data_mem_write(uint32_t w_val, uint32_t dest_addr, mem_width_t width,
                    data_memory *data_mem);

uint32_t data_mem_read(uint32_t addr, bool zero_extend, mem_width_t width,
                       data_memory *data_mem);

// Lists non-zero words as "[0xADDR]=value", used for the end-of-run summary
void data_mem_render(const data_memory *data_mem, char *buf, size_t cap);
