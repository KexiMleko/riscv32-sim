#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  char *file_path;
  enum { MODE_IN_ORDER, MODE_OOO } mode;
  uint32_t dmem_size;
  uint32_t imem_size;
  // bool debug_log;
} sim_config;

void parse_config(int argc, char *argv[], sim_config *cfg);
