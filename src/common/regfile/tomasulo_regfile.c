#include "regfile/tomasulo_regfile.h"
#include <stdint.h>
typedef struct {
  int32_t value;
  uint32_t producer_tag;
} regfile_entry;

static regfile_entry regfile[32];

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
