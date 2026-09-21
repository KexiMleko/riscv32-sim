#pragma once
#include <stdbool.h>

/*
 * Unit counts and CDB tag ranges. A tag identifies its producer across all
 * unit types, so the ranges must not overlap: RS 1..8, LB 9..12, SB 13..16.
 * Slot numbers inside a unit stay 1-based (RS1, LB1, ...), tag = base + slot.
 */
#define RS_CNT 8
#define LB_CNT 4
#define SB_CNT 4
#define RS_TAG_BASE 0
#define LB_TAG_BASE (RS_TAG_BASE + RS_CNT)
#define SB_TAG_BASE (LB_TAG_BASE + LB_CNT)

typedef struct {
  unsigned tag;
  int value;
  bool valid;
} CDB;
