#pragma once
#include "./alu_op.h"
#include "imm_type.h"
#include "mem_addressing_ctrl.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool rd_we;
  bool data_mem_write_en;
  bool data_mem_read_en;
  bool mem_to_reg;
  bool branch;
  bool alu_src_imm;
  alu_op_t alu_op;
  imm_type_t imm_type;
  mem_addressing_ctrl_t mem_addressing_ctrl;
} control_signals;

control_signals get_control_signals(uint32_t opcode, uint32_t funct3,
                                    uint32_t funct7);
