#include "./alu_ops.h"
#include <stdint.h>

alu_op op_alu_decode(uint32_t funct3, uint32_t funct7);
alu_op op_imm_alu_decode(uint32_t funct3, uint32_t funct7);
