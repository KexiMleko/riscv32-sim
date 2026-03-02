#pragma once

#include "memory/memory.h"
#include <stdbool.h>
#include <stdint.h>

bool run_inorder_pipeline(int32_t PC, instr_memory *instr_mem,
                          data_memory *data_mem);

bool run_ooo_pipeline(int32_t PC, instr_memory *instr_mem,
                      data_memory *data_mem);
