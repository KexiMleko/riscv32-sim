#include "./register_bank.h"

static uint32_t gp_regs[32] = {0};
static register_bank r_bank = {0};

void send_register_bank_input(uint32_t rs1_addr, uint32_t rs2_addr,
                              uint32_t rd_addr, bool rd_we) {
  r_bank.rs1_addr = rs1_addr;
  r_bank.rs2_addr = rs2_addr;
  r_bank.rs1_data = gp_regs[rs1_addr];
  r_bank.rs2_data = gp_regs[rs2_addr];
  r_bank.rd_addr=rd_addr;
  r_bank.rd_we = rd_we;
}

register_bank_output get_register_bank_output() {
  register_bank_output rb_out;
  rb_out.rs1_data = r_bank.rs1_data;
  rb_out.rs2_data = r_bank.rs2_data;
  return rb_out;
}
void rd_write(int32_t value) {
  if (r_bank.rd_we) {
    if (r_bank.rd_addr == 0)
      return;
    gp_regs[r_bank.rd_addr] = value;
  }
}
