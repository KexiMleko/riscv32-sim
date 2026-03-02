#include "common/alu.h"
#include "common/memory/memory.h"
#include "common/prog_load.h"
#include "config/config.h"
#include "config/params.h"
#include "pipeline_interface.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t PC = 0;
int main(int argc, char *argv[]) {
  if (argc == 0) {
    perror("Must provide path to the assembly file or a rv32i binary with "
           "instructions only");
    return EXIT_FAILURE;
  }
  sim_config cfg;
  parse_config(argc, argv, &cfg);

  uint32_t imem_size = cfg.imem_size == 0 ? DEFAULT_IMEM_SIZE : cfg.imem_size;
  uint32_t dmem_size = cfg.dmem_size == 0 ? DEFAULT_DMEM_SIZE : cfg.dmem_size;
  instr_memory *instr_mem =
      malloc(sizeof(uint32_t) * imem_size + sizeof(imem_size));
  data_memory *data_mem =
      malloc(sizeof(uint32_t) * dmem_size + sizeof(dmem_size));
  instr_mem->size = imem_size;
  data_mem->size = dmem_size;

  load_instructions(cfg.file_path, instr_mem);
  bool exit_status;
  switch (cfg.mode) {
  case MODE_IN_ORDER:
    exit_status = run_inorder_pipeline(PC, instr_mem, data_mem);
    break;
  case MODE_OOO:
    exit_status = run_ooo_pipeline(PC, instr_mem, data_mem);
    break;
  default:
    exit_status = run_inorder_pipeline(PC, instr_mem, data_mem);
  }
  free(instr_mem);
  free(data_mem);
  return exit_status;
}
