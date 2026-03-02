#pragma once
#include "memory/memory.h"
#include <stddef.h>
#include <stdint.h>

size_t load_instructions(char *file_path, instr_memory *instr_mem);
