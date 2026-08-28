#include "branch_prediction/branch_prediction_unit.h"
#include "config/params.h"
#include "control_decoder.h"
#include "forwarding/forwarding_unit.h"
#include "hazard_detection/hazard_detection_unit.h"
#include "instr_fields.h"
#include "memory/memory.h"
#include "pipe_regs.h"
#include "pipeline.h"
#include "pipeline_interface.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool run_inorder_pipeline(int32_t PC, instr_memory *instr_mem,
                          data_memory *data_mem) {

  uint64_t clk_cnt = 0;

  IF_ID if_id_reg = {0};
  ID_EX id_ex_reg = {0};
  EX_MEM ex_mem_reg = {0};
  MEM_WB mem_wb_reg = {0};
  branch_ctrl b_ctrl = {.next_pc = 0, .pc_next_sel = false};
  halt_signal halt = false;

  printf("\nPress Enter to cycle...\n");
  while (!halt) {
    clk_cnt++;
    bool b_flush = false;

    if (!mem_wb_reg.halt_signal) {
      fflush(stdout);
      (void)getchar();
    }

    if (clk_cnt >= MAX_CLK_COUNT) {
      printf("\nMax clock cycle count reached: Stopping simulation\n");
      return EXIT_FAILURE;
    }

    control_signals id_ctrl_peek = get_control_signals(
        get_opcode(if_id_reg.instr), get_funct3(if_id_reg.instr),
        get_funct7(if_id_reg.instr));
    hd_ctrl hd_ctrl = {
        .mem_to_reg_ex = id_ex_reg.ctrl.mem_to_reg,
        .rd_we_ex = id_ex_reg.ctrl.rd_we,
        .load_reg_dest_ex = id_ex_reg.rd_addr,
        .rs1_id = get_rs1(if_id_reg.instr),
        .rs2_id = get_rs2(if_id_reg.instr),
        .rs1_in_use_id = id_ctrl_peek.rs1_in_use,
        .rs2_in_use_id = id_ctrl_peek.rs2_in_use,
    };
    struct hd_signals hd = eval_hazard_detection(hd_ctrl);

    fw_ctrl fw_ctrl = {
      .rd_addr_mem = ex_mem_reg.rd_addr,
      .rd_addr_wb = mem_wb_reg.rd_addr,
      .rs1_addr = id_ex_reg.ctrl.rs1_addr,
      .rs2_addr = id_ex_reg.ctrl.rs2_addr,
      .rd_we_mem = ex_mem_reg.ctrl.rd_we,
      .rd_we_wb = mem_wb_reg.ctrl.rd_we,
    };
    struct fw_data fw_data = {
      .fw_signals = eval_forwarding(fw_ctrl),
      .alu_res_mem = ex_mem_reg.alu_res,
      .alu_res_wb = mem_wb_reg.ctrl.mem_to_reg ? mem_wb_reg.data_o
                                                : mem_wb_reg.alu_res,
    };

    const IF_ID if_id_next = instr_fetch(if_id_reg, instr_mem, PC);
    const ID_EX id_ex_next = instr_decode(if_id_reg, &b_ctrl, &b_flush);
    const EX_MEM ex_mem_next = execute(id_ex_reg, fw_data);
    const MEM_WB mem_wb_next = memory_access(ex_mem_reg, data_mem);
    halt = write_back(mem_wb_reg);

    /*
     * "Register" writes are performed after instruction stages to simulate
     * sequential logic
     */
    if (b_flush) {
      if_id_reg = (IF_ID){0};
      if_id_reg.b_ctrl = b_ctrl;
    } else if (hd.if_id_en) {
      if_id_reg = if_id_next;
    }

    if (hd.ctrl_pass) {
      id_ex_reg = id_ex_next;
    } else {
      id_ex_reg = (ID_EX){0};
    }
    ex_mem_reg = ex_mem_next;
    mem_wb_reg = mem_wb_next;
    seq_write_btb();
    if (hd.pc_en) {
      PC = if_id_reg.pc;
    }
    printf("-----------------------------\n");
  }
  printf("Cycle count: %lu\n", clk_cnt);
  return EXIT_SUCCESS;
}
