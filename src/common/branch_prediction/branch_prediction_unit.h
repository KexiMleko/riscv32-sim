#include <stdbool.h>
#include <stdint.h>

uint32_t perform_prediction(uint32_t PC);
void record_branch_result(uint32_t PC, uint32_t target, bool taken);
void seq_write_btb(); 
