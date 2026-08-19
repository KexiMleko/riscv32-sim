#include "memory/memory.h"
#include "pipe_regs.h"
#include "reservation_station/reservation_station.h"

IF_ID instr_fetch(IF_ID in, instr_memory *instr_mem, uint32_t pc);
void issue_instr(reservation_station *r_stations, unsigned int rs_cnt);
void execute();
void write_result();
