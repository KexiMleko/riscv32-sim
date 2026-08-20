#pragma once
//#include "alu_op.h"
#include <stdint.h>
// typedef struct {
//   alu_op_t op;
//   uint32_t rs1;
//   uint32_t rs2;
//   uint32_t rd;
//   uint32_t imm;
// } instr_t;

typedef enum {
  QUEUE_OK,
  QUEUE_FULL,
  QUEUE_EMPTY,
} queue_status;

queue_status dequeue_instr();
queue_status enqueue_instr(uint32_t instr);
queue_status instr_queue_front(uint32_t *instr);
