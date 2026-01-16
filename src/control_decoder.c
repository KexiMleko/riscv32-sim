#include "./control_decoder.h"
#include "./alu_decoder.h"
#include "./opcode.h"
#include "alu_op.h"
#include "imm_type.h"
#include "mem_addressing_ctrl.h"
#include <stdbool.h>
#include <stdint.h>

mem_addressing_ctrl_t decode_mem_addressing(uint32_t funct3) {
  mem_addressing_ctrl_t mem_ctrl = {0};
  switch (funct3) {
  case 0x0:
    mem_ctrl.addressing_width = MEM_WIDTH_BYTE;
    break;
  case 0x1:
    mem_ctrl.addressing_width = MEM_WIDTH_HALFWORD;
    break;
  case 0x2:
    mem_ctrl.addressing_width = MEM_WIDTH_WORD;
    break;
  case 0x4:
    mem_ctrl.addressing_width = MEM_WIDTH_BYTE;
    mem_ctrl.zero_extend = true;
    break;
  case 0x5:
    mem_ctrl.addressing_width = MEM_WIDTH_HALFWORD;
    mem_ctrl.zero_extend = true;
    break;
  }
  return mem_ctrl;
}
control_signals get_control_signals(uint32_t opcode, uint32_t funct3,
                                    uint32_t funct7) {
  control_signals ctrl = {0};
  switch (opcode) {
  case OPCODE_OP:
    ctrl.rd_we = true;
    ctrl.imm_type = IMM_NONE;
    ctrl.alu_op = op_alu_decode(funct3, funct7);
    break;
  case OPCODE_OP_IMM:
    ctrl.rd_we = true;
    ctrl.alu_src_imm = true;
    ctrl.imm_type = IMM_I;
    ctrl.alu_op = op_imm_alu_decode(funct3, funct7);
    break;
  case OPCODE_AUIPC:
    ctrl.imm_type = IMM_U;
    break;
  case OPCODE_BRANCH:
    ctrl.branch = true;
    ctrl.imm_type = IMM_B;
    ctrl.alu_op = ALU_NOP;
    break;
  case OPCODE_JAL:
    ctrl.imm_type = IMM_J;
    break;
  case OPCODE_JALR:
    ctrl.imm_type = IMM_I;
    break;
  case OPCODE_LOAD:
    ctrl.rd_we = true;
    ctrl.mem_to_reg = true;
    ctrl.alu_src_imm = true;
    ctrl.imm_type = IMM_I;
    ctrl.alu_op = ALU_ADD;
    ctrl.data_mem_read_en = true;
    ctrl.mem_addressing_ctrl = decode_mem_addressing(funct3);
    break;
  case OPCODE_STORE:
    ctrl.alu_src_imm = true;
    ctrl.data_mem_write_en = true;
    ctrl.imm_type = IMM_S;
    ctrl.alu_op = ALU_ADD;
    ctrl.mem_addressing_ctrl = decode_mem_addressing(funct3);
    break;
  case OPCODE_LUI:
    ctrl.rd_we = true;
    ctrl.imm_type = IMM_U;
    ctrl.alu_src_imm = true;
    break;
  case OPCODE_SYSTEM:
    break;
  }
  return ctrl;
}
