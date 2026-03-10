#pragma once
#include "common/pipe_regs.h"
#include "forwarding/forwarding_unit.h"
#include "memory/memory.h"
typedef bool halt_signal;

EX_MEM execute(ID_EX in, struct fw_data fw_data);
ID_EX instr_decode(IF_ID in, branch_ctrl *b_ctrl, bool *b_flush);
IF_ID instr_fetch(IF_ID in, instr_memory *instr_mem, uint32_t pc);
MEM_WB memory_access(EX_MEM in, data_memory *data_mem);
halt_signal write_back(MEM_WB in);
