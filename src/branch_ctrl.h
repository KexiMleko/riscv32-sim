#pragma once
#include <stdbool.h>
#include <stdint.h>

bool eval_branch(uint32_t rs1_val, uint32_t rs2_val, uint8_t funct3);
