#include "forwarding_unit.h"
#include <stdint.h>

struct fw_signals eval_forwarding(fw_ctrl fw_ctrl) {

  struct fw_signals out = {FW_NO_DEPS, FW_NO_DEPS};
  if (fw_ctrl.rd_we_mem && fw_ctrl.rd_addr_mem != 0 &&
      fw_ctrl.rd_addr_mem == fw_ctrl.rs1_addr) {
    out.alu_forward_a = FW_MEM_RES;
  } else if (fw_ctrl.rd_we_wb && fw_ctrl.rd_addr_wb != 0 &&
             fw_ctrl.rd_addr_wb == fw_ctrl.rs1_addr) {
    out.alu_forward_a = FW_WB_RES;
  }

  if (fw_ctrl.rd_we_mem && fw_ctrl.rd_addr_mem != 0 &&
      fw_ctrl.rd_addr_mem == fw_ctrl.rs2_addr) {
    out.alu_forward_b = FW_MEM_RES;
  } else if (fw_ctrl.rd_we_wb && fw_ctrl.rd_addr_wb != 0 &&
             fw_ctrl.rd_addr_wb == fw_ctrl.rs2_addr) {
    out.alu_forward_b = FW_WB_RES;
  }

  return out;
}
