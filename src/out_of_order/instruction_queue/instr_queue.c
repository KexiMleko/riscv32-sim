#include "instr_queue.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_QUEUE_SIZE 8
iq_entry instr_queue[MAX_QUEUE_SIZE];
uint32_t size;

bool iq_is_empty() { return size == 0; }
bool iq_is_full() { return size == MAX_QUEUE_SIZE - 1; }

queue_status enqueue_instr(uint32_t instr, uint32_t pc) {
  if (iq_is_full()) {
    return QUEUE_FULL;
  }
  instr_queue[size] = (iq_entry){.instr = instr, .pc = pc};
  size++;
  printf("instructions queued: %u", size);
  return QUEUE_OK;
}
queue_status iq_front(iq_entry *out) {
  if (iq_is_empty())
    return QUEUE_EMPTY;
  *out = instr_queue[0];
  return QUEUE_OK;
}

queue_status dequeue_instr() {
  if (iq_is_empty())
    return QUEUE_EMPTY;

  for (size_t i = 0; i < size - 1; i++) {
    instr_queue[i] = instr_queue[i + 1];
  }
  instr_queue[size] = (iq_entry){0};
  size--;
  return QUEUE_OK;
}
void iq_flush() {
  for (size_t i = 0; i < size; i++) {
    instr_queue[i] = (iq_entry){0};
  }
  size = 0;
}
