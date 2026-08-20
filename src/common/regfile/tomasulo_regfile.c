#include "regfile/tomasulo_regfile.h"
#include "common_data_bus/common_data_bus.h"
#include <stdint.h>
#include <stdio.h>
#define NUM_REGS 32
typedef struct {
  int32_t value;
  uint32_t producer_tag;
} regfile_entry;

static regfile_entry regfile[NUM_REGS];

uint32_t regfile_read_tag(uint32_t rd_addr) {
  if (rd_addr == 0) {
    return 0;
  }
  return regfile[rd_addr].producer_tag;
}
int32_t regfile_read_val(uint32_t rd_addr) {
  if (rd_addr == 0) {
    return 0;
  }
  return regfile[rd_addr].value;
}
void regfile_write(uint32_t value, uint32_t rd_addr) {
  if (rd_addr == 0) {
    return;
  }
  regfile[rd_addr].value = value;
}
void regfile_update_tag(uint32_t rd_addr, uint32_t tag) {
  if (rd_addr == 0) {
    return;
  }
  regfile[rd_addr].producer_tag = tag;
}
void regfile_cdb_snoop(CDB cdb) {
  if (!cdb.valid)
    return;
  for (int r = 0; r < NUM_REGS; r++) {
    if (regfile[r].producer_tag == cdb.tag && cdb.tag != 0) {
      printf("[WB]  REG[%u] resolved (val=%d, tag=%d)\n", r, cdb.value,
             cdb.tag);
      regfile[r].value = cdb.value;
      regfile[r].producer_tag = 0;
    }
  }
}
