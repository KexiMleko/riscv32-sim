#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  QUEUE_OK,
  QUEUE_FULL,
  QUEUE_EMPTY,
} queue_status;

typedef struct {
  uint32_t instr;
  uint32_t pc;
} iq_entry;

bool iq_is_empty();
bool iq_is_full();
queue_status dequeue_instr();
queue_status enqueue_instr(uint32_t instr, uint32_t pc);
queue_status iq_front(iq_entry *out);
void iq_flush();
