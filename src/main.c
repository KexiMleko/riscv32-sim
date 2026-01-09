#include "./alu.h"
#include "./branch_ctrl.h"
#include "./control_decoder.h"
#include "./instr_fields.h"
#include "./prog_load.h"
#include "./register_bank.h"
#include "imm_gen.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t pc = 0;
uint32_t instr_mem[1024] = {0};
uint32_t data_mem[1024] = {0};

static inline uint32_t fetch_next_instruction() { return instr_mem[pc >> 2]; }
int main(int argc, char *args[]) {
  if (argc == 0) {
    perror("Must provide path to the assembly file or a rv32i binary with "
           "instructions only");
    return 1;
  }
  char *file_path = args[1];
  size_t instr_count = load_instructions(file_path, instr_mem);

  const uint32_t total_instr_size = instr_count * 4;
  while (pc < total_instr_size) {
    uint32_t next_pc = pc + 4;
    uint32_t instr = fetch_next_instruction();
    printf("\nPC: %d\nInstruction started: %08x\n", pc, instr);

    control_signals ctrl = get_control_signals(
        get_opcode(instr), get_funct3(instr), get_funct7(instr));

    send_register_bank_input(get_rs1(instr), get_rs2(instr), get_rd(instr),
                             ctrl.rd_we);

    uint32_t imm = generate_imm(instr, ctrl.imm_type);
    register_bank_output rb_out = get_register_bank_output();
    uint32_t rs1 = rb_out.rs1_data;
    uint32_t rs2 = ctrl.alu_src_imm ? imm : rb_out.rs2_data;

    if (ctrl.branch) {
      if (eval_branch(rs1, rs2, get_funct3(instr))) {
        next_pc = pc + imm;
        printf("branching taken, pc is %d\n", pc);
      } else {
        printf("branching not taken, pc is %d\n", pc);
      }
    } else {

      int32_t alu_result = execute_alu(rs1, rs2, ctrl.alu_op);

      if (ctrl.data_mem_we) {
        data_mem[alu_result] = rs1;
      }
      uint32_t reg_wr_data;
      if (ctrl.mem_to_reg) {
        reg_wr_data = data_mem[alu_result];
      } else {
        reg_wr_data = alu_result;
      }
      rd_write(reg_wr_data);
    }
    pc = next_pc;
  }
}
