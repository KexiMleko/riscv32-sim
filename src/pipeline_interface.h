#pragma once

#include <stdbool.h>
#include <stdint.h>

bool run_inorder_pipeline(int32_t PC, uint64_t MAX_CLK_COUNT);
bool run_ooo_pipeline(int32_t PC, uint64_t MAX_CLK_COUNT);
