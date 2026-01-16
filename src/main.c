#include "./alu.h"
#include "./pipe_regs.h"
#include "./prog_load.h"
#include "./stages/execute_stage.c"
#include "./stages/instr_decode_stage.c"
#include "./stages/instr_fetch_stage.c"
#include "./stages/memory_access_stage.c"
#include "./stages/write_back_stage.c"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_CLK_COUNT 5000
uint32_t PC = 0;
uint32_t instr_mem[1024] = {0};
uint32_t data_mem[1024] = {0};

int main(int argc, char *args[]) {
  if (argc == 0) {
    perror("Must provide path to the assembly file or a rv32i binary with "
           "instructions only");
    return 1;
  }
  char *file_path = args[1];
  load_instructions(file_path, instr_mem);

  uint64_t clk_cycle = 0;

  IF_ID if_id_reg = {0};
  ID_EX id_ex_reg = {0};
  EX_MEM ex_mem_reg = {0};
  MEM_WB mem_wb_reg = {0};
  halt_signal halt = false;

  while (!halt) {
    clk_cycle++;
    if (!mem_wb_reg.halt_signal) {
      printf("\nPress Enter to cycle...");
      fflush(stdout);
      (void)getchar();
      printf("\nClock cycle: %lu\n", clk_cycle);
    }

    if (clk_cycle >= MAX_CLK_COUNT) {
      printf("\nMax clock cycle count reached: Stopping simulation\n");
      break;
    }

    halt = write_back(mem_wb_reg);
    mem_wb_reg = memory_access(ex_mem_reg);
    ex_mem_reg = execute(id_ex_reg);
    id_ex_reg = instr_decode(if_id_reg);
    if_id_reg = instr_fetch(if_id_reg, PC);
    PC = if_id_reg.pc;
  }
}
