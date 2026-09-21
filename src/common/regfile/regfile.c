#include "regfile.h"
#include <stdint.h>
#include <stdio.h>
#include "trace.h"

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

void regfile_render(char *buf, size_t cap) {
  buf[0] = '\0';
  for (unsigned r = 1; r < 32; r++) {
    if (gp_regs[r] != 0)
      trace_appendf(buf, cap, "%sx%u=%d", buf[0] ? "  " : "", r,
                    (int32_t)gp_regs[r]);
  }
}
