#include "instr_queue.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_QUEUE_SIZE 8
uint32_t instr_queue[MAX_QUEUE_SIZE];
uint32_t size;

bool is_empty() { return size == 0; }
bool is_full() { return size == MAX_QUEUE_SIZE - 1; }

queue_status enqueue_instr(uint32_t instr) {
  if (is_full()) {
    return QUEUE_FULL;
  }
  instr_queue[size] = instr;
  size++;
  printf("instructions queued: %u", size);
  return QUEUE_OK;
}
queue_status instr_queue_front(uint32_t *instr) {
  if (is_empty())
    return QUEUE_EMPTY;
  *instr = instr_queue[0];
  return QUEUE_OK;
}

queue_status dequeue_instr() {
  if (is_empty())
    return QUEUE_EMPTY;

  for (size_t i = 0; i < size - 1; i++) {
    instr_queue[i] = instr_queue[i + 1];
  }
  instr_queue[size] = 0;
  size--;
  return QUEUE_OK;
}
