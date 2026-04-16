#pragma once
#include <stdbool.h>
#include <stdint.h>


uint32_t regfile_read(uint32_t rs1_addr);
void regfile_write(uint32_t value, uint32_t rd_addr);
