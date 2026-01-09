#include "./alu_ops.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum { IMM_NONE, IMM_I, IMM_S, IMM_B, IMM_U, IMM_J } imm_type_t;

typedef struct {
  bool rd_we;
  bool data_mem_we;
  bool mem_to_reg;
  bool branch;
  bool alu_src_imm;
  alu_op alu_op;
  imm_type_t imm_type;
} control_signals;

control_signals get_control_signals(uint32_t opcode, uint32_t funct3,
                                    uint32_t funct7);
