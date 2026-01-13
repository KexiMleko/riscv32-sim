#pragma once
#include "alu_op.h"
#include "control_decoder.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint32_t instr;
  uint32_t pc;
  int32_t next_pc;
  bool pc_next_sel; // 0 - add result; 1 - branch res (next_pc)
  bool halt_signal;
} IF_ID;

typedef struct {
  uint32_t val1;
  uint32_t val2;
  int32_t imm;
  uint32_t rd_addr;
  control_signals ctrl;
  bool halt_signal;
} ID_EX;

typedef struct {
  uint32_t alu_res;
  uint32_t data_i;
  uint32_t rd_addr;
  control_signals ctrl;
  bool halt_signal;
} EX_MEM;

typedef struct {
  uint32_t alu_res;
  uint32_t data_o;
  uint32_t rd_addr;
  control_signals ctrl;
  bool halt_signal;
} MEM_WB;
