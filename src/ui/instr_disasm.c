#include "instr_disasm.h"
#include "instr_fields.h"
#include "opcode.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void instr_disasm(uint32_t instr, char *out, size_t len) {
  uint32_t op = get_opcode(instr);
  uint32_t rd = get_rd(instr);
  uint32_t rs1 = get_rs1(instr);
  uint32_t rs2 = get_rs2(instr);
  uint32_t funct3 = get_funct3(instr);
  uint32_t funct7 = get_funct7(instr);

  switch (op) {

  case OPCODE_LUI:
    snprintf(out, len, "lui x%u, 0x%x", rd, get_u_imm(instr));
    break;
  case OPCODE_AUIPC:
    snprintf(out, len, "auipc x%u, 0x%x", rd, get_u_imm(instr));
    break;
  case OPCODE_JAL:
    snprintf(out, len, "jal x%u, %d", rd, get_j_imm(instr));
    break;
  case OPCODE_JALR:
    snprintf(out, len, "jalr x%u, %d(x%u)", rd, get_i_imm(instr), rs1);
    break;
  case OPCODE_BRANCH: {
    int32_t imm = get_b_imm(instr);
    const char *mn = (funct3 == 0x0)   ? "beq"
                     : (funct3 == 0x1) ? "bne"
                     : (funct3 == 0x4) ? "blt"
                     : (funct3 == 0x5) ? "bge"
                     : (funct3 == 0x6) ? "bltu"
                     : (funct3 == 0x7) ? "bgeu"
                                       : "unknown";

    snprintf(out, len, "%s x%u, x%u, %d", mn, rs1, rs2, imm);
    break;
  }
  case OPCODE_LOAD: {
    int32_t imm = get_i_imm(instr);
    const char *mn = (funct3 == 0x0)   ? "lb"
                     : (funct3 == 0x1) ? "lh"
                     : (funct3 == 0x2) ? "lw"
                     : (funct3 == 0x4) ? "lbu"
                     : (funct3 == 0x5) ? "lhu"
                                       : "unknown";

    snprintf(out, len, "%s x%u, %d(x%u)", mn, rd, imm, rs1);
    break;
  }
  case OPCODE_STORE: {
    int32_t imm = get_s_imm(instr);
    const char *mn = (funct3 == 0x0)   ? "sb"
                     : (funct3 == 0x1) ? "sh"
                     : (funct3 == 0x2) ? "sw"
                                       : "unknown";

    snprintf(out, len, "%s x%u, %d(x%u)", mn, rs2, imm, rs1);
    break;
  }
  case OPCODE_OP_IMM: {
    int32_t imm = get_i_imm(instr);
    const char *mn = (funct3 == 0x0)                     ? "addi"
                     : (funct3 == 0x2)                   ? "slti"
                     : (funct3 == 0x3)                   ? "sltiu"
                     : (funct3 == 0x4)                   ? "xori"
                     : (funct3 == 0x6)                   ? "ori"
                     : (funct3 == 0x7)                   ? "andi"
                     : (funct3 == 0x1)                   ? "slli"
                     : (funct3 == 0x5 && funct7 == 0x00) ? "srli"
                     : (funct3 == 0x5 && funct7 == 0x20) ? "srai"
                                                         : "unknown";

    if (!strcmp(mn, "slli") || !strcmp(mn, "srli") || !strcmp(mn, "srai"))
      snprintf(out, len, "%s x%u, x%u, %u", mn, rd, rs1, imm & 0x1F);
    else
      snprintf(out, len, "%s x%u, x%u, %d", mn, rd, rs1, imm);
    break;
  }
  case OPCODE_OP: {
    const char *mn = (funct3 == 0x0 && funct7 == 0x00)   ? "add"
                     : (funct3 == 0x0 && funct7 == 0x20) ? "sub"
                     : (funct3 == 0x1)                   ? "sll"
                     : (funct3 == 0x2)                   ? "slt"
                     : (funct3 == 0x3)                   ? "sltu"
                     : (funct3 == 0x4)                   ? "xor"
                     : (funct3 == 0x5 && funct7 == 0x00) ? "srl"
                     : (funct3 == 0x5 && funct7 == 0x20) ? "sra"
                     : (funct3 == 0x6)                   ? "or"
                     : (funct3 == 0x7)                   ? "and"
                                                         : "unknown";

    snprintf(out, len, "%s x%u, x%u, x%u", mn, rd, rs1, rs2);
    break;
  }
  case OPCODE_SYSTEM:
    if (instr == 0x00000073)
      snprintf(out, len, "ecall");
    else if (instr == 0x00100073)
      snprintf(out, len, "ebreak");
    else
      snprintf(out, len, "system");
    break;

  default:
    snprintf(out, len, "<unknown 0x%08x>", instr);
    break;
  }
}
