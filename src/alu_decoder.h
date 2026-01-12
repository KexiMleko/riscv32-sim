#include "./alu_op.h"
#include <stdint.h>

alu_op_t op_alu_decode(uint32_t funct3, uint32_t funct7);
alu_op_t op_imm_alu_decode(uint32_t funct3, uint32_t funct7);
