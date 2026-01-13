#include "regfile.h"
#include "./regfile.h"
#include <stdint.h>
#include <stdio.h>

static uint32_t gp_regs[32] = {0};

regfile_out regfile_read(uint32_t rs1_addr, uint32_t rs2_addr) {
  regfile_out rb_out;
  rb_out.rs1_data = gp_regs[rs1_addr];
  rb_out.rs2_data = gp_regs[rs2_addr];
  return rb_out;
}

void rd_write(uint32_t value, uint32_t rd_addr) {
  if (rd_addr >= 32) {
    printf("Invalid register address: %d\n", rd_addr);
    return;
  }
  if (rd_addr == 0)
    return;
  gp_regs[rd_addr] = value;
}
