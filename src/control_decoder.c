#include "./control_decoder.h"
#include "./alu_decoder.h"
#include "./opcodes.h"
#include "alu_ops.h"
#include <stdbool.h>
#include <stdint.h>

control_signals get_control_signals(uint32_t opcode, uint32_t funct3,
                                    uint32_t funct7) {
  control_signals ctrl = {0};
  switch (opcode) {
  case OPCODE_OP:
    ctrl.rd_we = true;
    ctrl.alu_op = op_alu_decode(funct3, funct7);
    break;
  case OPCODE_OP_IMM:
    ctrl.rd_we = true;
    ctrl.alu_src_imm = true;
    ctrl.alu_op = op_imm_alu_decode(funct3, funct7);
    break;
  case OPCODE_AUIPC:
    break;
  case OPCODE_BRANCH:
    break;
  case OPCODE_JAL:
    break;
  case OPCODE_JALR:
    break;
  case OPCODE_LOAD:
    ctrl.rd_we = true;
    ctrl.mem_to_reg = true;
    ctrl.alu_src_imm = true;
    ctrl.alu_op = ALU_ADD;
    break;
  case OPCODE_STORE:
    ctrl.alu_src_imm = true;
    ctrl.data_mem_we = true;
    ctrl.alu_op = ALU_ADD;
    break;
  case OPCODE_LUI:
      ctrl.rd_we=true;
      ctrl.alu_src_imm=true;
    break;
  case OPCODE_SYSTEM:
    break;
  }
  return ctrl;
}
