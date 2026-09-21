#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint32_t rs1_data;
  uint32_t rs2_data;
} regfile_out;

regfile_out regfile_read(uint32_t rs1_addr, uint32_t rs2_addr);
void rd_write(uint32_t value, uint32_t rd_addr);
void regfile_render(char *buf, size_t cap);
