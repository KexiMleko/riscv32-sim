#include <stdint.h>
typedef struct {
  uint32_t value;
  uint32_t producer_tag ;
} regfile_entry;

static regfile_entry regfile[32];




