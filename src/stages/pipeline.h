#pragma once
#include "../pipe_regs.h"
typedef bool halt_signal;

EX_MEM execute(ID_EX in);
ID_EX instr_decode(IF_ID in, branch_ctrl *b_ctrl);
IF_ID instr_fetch(IF_ID in, uint32_t pc);
MEM_WB memory_access(EX_MEM in);
halt_signal write_back(MEM_WB in);
