#include <stdbool.h>
#include <stdint.h>
typedef struct {
  bool mem_to_reg_ex;
  bool rd_we_ex;
  uint32_t load_reg_dest_ex;
  uint32_t rs1_id;
  uint32_t rs2_id;
  bool rs1_in_use_id;
  bool rs2_in_use_id;
} hd_ctrl;

struct hd_signals {
  bool pc_en;
  bool if_id_en;
  bool ctrl_pass;
};

struct hd_signals eval_hazard_detection(hd_ctrl ctrl);
