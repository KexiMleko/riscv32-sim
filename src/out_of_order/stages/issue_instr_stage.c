#include "control_decoder.h"
#include "instr_fields.h"
#include "instruction_queue/instr_queue.h"
#include "ooo_pipeline.h"
#include "regfile/tomasulo_regfile.h"
#include "reservation_station/reservation_station.h"
#include <stdint.h>

issue_result issue_instr(reservation_station *r_stations, unsigned int rs_cnt) {
  uint32_t instr = 0;
  issue_result res = {0};
  if (dequeue_instr(&instr) != QUEUE_OK) {
    return res;
  }
  uint32_t opcode = get_opcode(instr);
  uint32_t funct3 = get_funct3(instr);
  uint32_t funct7 = get_funct7(instr);
  uint32_t rs1 = get_rs1(instr);
  uint32_t rs2 = get_rs2(instr);
  uint32_t rd = get_rd(instr);

  control_signals ctrl = get_control_signals(opcode, funct3, funct7);

  for (unsigned i = 0; i < rs_cnt; i++) {
    if (!r_stations[i].busy) {
      reservation_station rs = r_stations[i];
      rs.op = opcode;

      if (regfile_read_tag(rs1) == 0) {
        rs.rs1_val = regfile_read_val(rs1);
      } else {
        rs.rs1_tag = regfile_read_tag(rs1);
      }
      if (regfile_read_tag(rs2) == 0) {
        rs.rs2_val = regfile_read_val(rs2);
      } else {
        rs.rs1_tag = regfile_read_tag(rs1);
      }
      rs.busy = true;
      res.valid = true;
      res.rs = rs;
      res.tag = i;
      return res;
    }
  }
}
