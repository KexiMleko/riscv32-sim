#include "common/alu.h"
#include "common/prog_load.h"
#include "config/config.h"
#include "pipeline_interface.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CLK_COUNT 5000
uint32_t PC = 0;
uint32_t instr_mem[1024] = {0};
uint32_t data_mem[1024] = {0};

int main(int argc, char *argv[]) {
  if (argc == 0) {
    perror("Must provide path to the assembly file or a rv32i binary with "
           "instructions only");
    return EXIT_FAILURE;
  }
  sim_config cfg;
  parse_config(argc, argv, &cfg);
  load_instructions(cfg.file_path, instr_mem);
  bool exit_status;
  switch (cfg.mode) {
  case MODE_IN_ORDER:
    exit_status = run_inorder_pipeline(PC, MAX_CLK_COUNT);
    break;
  case MODE_OOO:
    exit_status = run_ooo_pipeline(PC, MAX_CLK_COUNT);
    break;
  default:
    exit_status = run_inorder_pipeline(PC, MAX_CLK_COUNT);
  }
  return exit_status;
}
