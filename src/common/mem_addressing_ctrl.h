#pragma once
#include <stdbool.h>

typedef enum {
  MEM_WIDTH_BYTE = 8,
  MEM_WIDTH_HALFWORD = 16,
  MEM_WIDTH_WORD = 32,
} mem_width_t;

typedef struct {
  mem_width_t addressing_width;
  bool zero_extend;
} mem_addressing_ctrl_t;
