#include "memory/memory.h"
#include "pipe_regs.h"
#include "reservation_station/reservation_station.h"

typedef struct {
  bool valid;
  uint8_t tag;
  reservation_station rs;
  uint32_t rd; //register destination for regfile update
} issue_result;

IF_ID instr_fetch(IF_ID in, instr_memory *instr_mem, uint32_t pc);
issue_result issue_instr(reservation_station *r_stations, unsigned int rs_cnt);
void execute();
void write_result();
