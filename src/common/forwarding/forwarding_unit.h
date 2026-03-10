#pragma once
#include <stdbool.h>
#include <stdint.h>
typedef struct {
  bool rd_we_mem;
  bool rd_we_wb;
  uint32_t rd_addr_mem;
  uint32_t rd_addr_wb;
  uint32_t rs1_addr;
  uint32_t rs2_addr;
} fw_ctrl;

enum fw_state {
  FW_NO_DEPS = 0b00,
  FW_WB_RES = 0b01,
  FW_MEM_RES = 0b10,
};

struct fw_signals {
  enum fw_state alu_forward_a;
  enum fw_state alu_forward_b;
};

struct fw_data {
  struct fw_signals fw_signals;
  uint32_t alu_res_mem;
  uint32_t alu_res_wb;
};

struct fw_signals eval_forwarding(fw_ctrl fw_ctrl);
