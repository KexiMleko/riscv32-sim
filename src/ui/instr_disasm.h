#pragma once
#include <stddef.h>
#include <stdint.h>

void instr_disasm(uint32_t instr, char *out, size_t len);
