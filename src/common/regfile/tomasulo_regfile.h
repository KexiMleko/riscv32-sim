#pragma once
#include <stdbool.h>
#include <stdint.h>

uint32_t regfile_read_tag(uint32_t rd_addr);
int32_t regfile_read_val(uint32_t rd_addr);
void regfile_write(uint32_t value, uint32_t rd_addr);
void regfile_update_tag(uint32_t rd_addr, uint32_t tag);
