#include "imm_type.h"
#include "imm_gen.h"
#include "instr_fields.h"
#include <stdint.h>

int32_t generate_imm(uint32_t instr, imm_type_t imm_type) {
  switch (imm_type) {
  case IMM_NONE:
    return 0;
  case IMM_I:
    return get_i_imm(instr);
  case IMM_S:
    return get_s_imm(instr);
  case IMM_B:
    return get_b_imm(instr);
  case IMM_U:
    return get_u_imm(instr);
  case IMM_J:
    return get_j_imm(instr);
  }
  return 0;
}
