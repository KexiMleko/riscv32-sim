#include "./alu.h"
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
  printf("\nLoaded %zu instructions into instruction memory\n", instr_count);
  fclose(file);
  return instr_count;
}
static inline uint32_t fetch_next_instruction() { return instr_mem[pc >> 2]; }

int main(int argc, char *args[]) {
  if (argc == 0) {
    perror("Must provide path to the assembly file or a rv32i binary with "
           "instructions only");
    return 1;
  }
  char *file_path = args[1];
  size_t instr_count = load_instructions(file_path);

  for (; pc < (instr_count * 4); pc += 4) {
    uint32_t instr = fetch_next_instruction();

    control_signals ctrl = get_control_signals(
        get_opcode(instr), get_funct3(instr), get_funct7(instr));

    send_register_bank_input(get_rs1(instr), get_rs2(instr), get_rd(instr),
                             ctrl.rd_we);

    uint32_t imm = get_i_imm(instr);
    register_bank_output rb_out = get_register_bank_output();
    uint32_t value1 = rb_out.rs1_data;
    uint32_t value2 = ctrl.alu_src_imm ? imm : rb_out.rs1_data;
    int32_t alu_result = execute_alu(value1, value2, ctrl.alu_op);
    if (ctrl.data_mem_we) {
      data_mem[alu_result] = value1;
    }
    uint32_t reg_wr_data;
    if (ctrl.mem_to_reg) {
      reg_wr_data = data_mem[alu_result];
    } else {
      reg_wr_data = alu_result;
    }
    rd_write(reg_wr_data);
    printf("\nInstruction finished: %08x\n", instr);
  }
}
