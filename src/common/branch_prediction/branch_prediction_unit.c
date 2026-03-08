#include "branch_prediction_unit.h"
#include <stdbool.h>
#include <stdint.h>

#define BHT_SIZE 256 // must be a base 2
#define BHT_MASK (BHT_SIZE - 1)
typedef struct {
  uint32_t pc;
  uint32_t target;
  uint8_t predictor;
  bool valid;
} btb_entry;

static btb_entry branch_target_buffer[BHT_SIZE];
static btb_entry last_entry;
static uint32_t last_index;
uint32_t perform_prediction(uint32_t PC) {
  uint32_t index = (PC >> 2) & BHT_MASK;
  btb_entry entry = branch_target_buffer[index];
  if (entry.valid && entry.predictor >= 0b10 && PC == entry.pc) {
    return entry.target;
  }
  return PC;// + 4;
}

void record_branch_result(uint32_t PC, uint32_t target, bool taken) {
  uint32_t index = (PC >> 2) & BHT_MASK;
  uint8_t predictor = branch_target_buffer[index].predictor;
  btb_entry entry = (btb_entry){PC, target, predictor, true};
  entry.target = target;
  if (taken) {
    if (entry.predictor != 0b11) {
      entry.predictor++;
    }
  } else {
    if (entry.predictor != 0b00) {
      entry.predictor--;
    }
  }
  last_index = index;
  last_entry = entry;
  //  branch_target_buffer[index] = entry;
}

void seq_write_btb() { branch_target_buffer[last_index] = last_entry; }
