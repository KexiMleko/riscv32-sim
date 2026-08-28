#include "hazard_detection_unit.h"

struct hd_signals eval_hazard_detection(hd_ctrl ctrl) {
  bool load_use =
      ctrl.rd_we_ex && ctrl.mem_to_reg_ex && ctrl.load_reg_dest_ex != 0 &&
      ((ctrl.rs1_in_use_id && ctrl.load_reg_dest_ex == ctrl.rs1_id) ||
       (ctrl.rs2_in_use_id && ctrl.load_reg_dest_ex == ctrl.rs2_id));

  if (load_use) {
    return (struct hd_signals){
        .pc_en = false, .if_id_en = false, .ctrl_pass = false};
  }
  return (struct hd_signals){
      .pc_en = true, .if_id_en = true, .ctrl_pass = true};
}
