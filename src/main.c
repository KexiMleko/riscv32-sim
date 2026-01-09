#include "./alu.h"
#include "./branch_ctrl.h"
#include "./control_decoder.h"
#include "./instr_fields.h"
#include "./register_bank.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define SH_SEP " && "
#define GCC "riscv-none-elf-gcc"
#define OBJCOPY "riscv-none-elf-objcopy"
#else
#define SH_SEP " ; "
#define GCC "riscv64-elf-gcc"
#define OBJCOPY "riscv64-elf-objcopy"
#endif

uint32_t pc = 0;
uint32_t instr_mem[1024] = {0};
uint32_t data_mem[1024] = {0};

size_t load_instructions(char *file_path) {
  if (file_path[strlen(file_path) - 1] == 's') {
    size_t cmd_len = 512;
    char cmd_buffer[cmd_len] = {};
    snprintf(cmd_buffer, cmd_len,
             "%s -march=rv32i -mabi=ilp32 -nostdlib -Ttext 0x0 %s "
             "-o output.elf%s"
             "%s -O binary -j .text output.elf output.bin",
             GCC, file_path, SH_SEP, OBJCOPY);
    printf("\nCompile and disassemble invoked via:\n      %s", cmd_buffer);
    int res = system(cmd_buffer);
    if (res != 0) {
      perror("Error compiling file");
      return 0;
    }
    file_path = "./output.bin";
  }
  FILE *file = fopen(file_path, "rb");
  if (file == NULL) {
    perror("Error opening file");
    return 0;
  }
  size_t instr_count = fread(instr_mem, sizeof(uint32_t), 1024, file);
  printf("\n\nLoaded %zu instructions into instruction memory\n\n",
         instr_count);
  fclose(file);
  return instr_count;
}
static inline uint32_t fetch_next_instruction() { return instr_mem[pc >> 2]; }
static inline int32_t generate_imm(uint32_t instr, imm_type_t imm_type) {
  switch (imm_type) {
  case IMM_NONE:
    return 0;
  case IMM_I:
    return get_i_imm(instr);
  case IMM_S:
    return get_s_imm(instr);
  case IMM_B:
    return get_b_imm(instr);
  case IMM_U:
    return get_u_imm(instr);
  case IMM_J:
    return get_j_imm(instr);
  }
  return 0;
}
int main(int argc, char *args[]) {
  if (argc == 0) {
    perror("Must provide path to the assembly file or a rv32i binary with "
           "instructions only");
    return 1;
  }
  char *file_path = args[1];
  size_t instr_count = load_instructions(file_path);

  while (pc < (instr_count * 4)) {
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
