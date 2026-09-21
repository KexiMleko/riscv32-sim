#include "alu.h"
#include "common_data_bus/common_data_bus.h"
#include "ls_buffers/load_buffers.h"
#include "ls_buffers/store_buffers.h"
#include "memory/memory.h"
#include "ooo_pipeline.h"
#include "reservation_station/reservation_station.h"
#include <stddef.h>
#include "trace.h"

exec_result ooo_execute(data_memory *dmem) {
  uint32_t lb_ready_tag = lb_find_ready();
  uint32_t sb_ready_tag = sb_find_ready();

  reservation_station rs;
  uint32_t rs_ready_tag = rs_find_ready(&rs);
  if (lb_ready_tag) {
    uint32_t val = lb_execute(lb_ready_tag, dmem);
    trace_row(TR_EXEC, "LB%u load", lb_ready_tag - LB_TAG_BASE);
    return (exec_result){
        .cdb = {.tag = lb_ready_tag, .valid = true, .value = val},
        .freed_unit = UNIT_LB,
        .freed_tag = lb_ready_tag,
    };

  } else if (sb_ready_tag) {
    trace_row(TR_EXEC, "SB%u store", sb_ready_tag - SB_TAG_BASE);
    sb_commit(sb_ready_tag, dmem);
    return (exec_result){
        .cdb = {.valid = false},
        .freed_unit = UNIT_SB,
        .freed_tag = sb_ready_tag,
    };

  } else if (rs_ready_tag) {
    int value = execute_alu(rs.rs1_val, rs.rs2_val, rs.op);
    trace_row(TR_EXEC, "RS%u %s(%d, %d)", rs_ready_tag, alu_op_str(rs.op),
              rs.rs1_val, rs.rs2_val);
    trace_note(TR_EXEC, "→ %d", value);
    return (exec_result){
        .cdb = {.tag = rs_ready_tag, .valid = true, .value = value},
        .freed_unit = UNIT_RS,
        .freed_tag = rs_ready_tag,
    };
  } else {
    return (exec_result){.cdb = {.valid = false}, .freed_unit = UNIT_NONE};
  }
}
