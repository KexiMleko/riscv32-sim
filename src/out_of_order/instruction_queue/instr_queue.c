#include "instr_queue.h"
#include <stdbool.h>
#include <stdint.h>

#define MAX_QUEUE_SIZE 8
static uint32_t instr_queue[MAX_QUEUE_SIZE];
static uint32_t size;

bool is_empty() { return size == 0; }
bool is_full() { return size == MAX_QUEUE_SIZE; }

queue_status enqueue_instr(uint32_t instr) {
  if (is_full()) {
    return QUEUE_FULL;
  }
  instr_queue[size] = instr;
  size++;
  return QUEUE_OK;
}
queue_status dequeue_instr(uint32_t *instr) {
  if (is_empty())
    return QUEUE_EMPTY;
  *instr = instr_queue[0];
  for (int i = 1; i < MAX_QUEUE_SIZE; i++) {
    instr_queue[i - 1] = instr_queue[i];
  }
  size--;
  return QUEUE_OK;
}
