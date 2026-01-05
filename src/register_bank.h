#pragma once
#include <stdbool.h>
#include <stdint.h>
typedef struct {
  uint32_t rs1_data;
  uint32_t rs2_data;
} register_bank_output;
typedef struct {
  uint32_t rs1_addr;
  uint32_t rs2_addr;
  uint32_t rd_addr;
  uint32_t rs1_data;
  uint32_t rs2_data;
  bool rd_we;
} register_bank;

void send_register_bank_input(uint32_t rs1_addr, uint32_t rs2_addr, uint32_t rd_addr,
                       bool rd_we);
register_bank_output get_register_bank_output();
void rd_write(uint32_t value);
