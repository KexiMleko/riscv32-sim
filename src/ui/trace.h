#pragma once
#include <stddef.h>
#include <stdint.h>

/*
 * Per-cycle trace shared by the in-order and Tomasulo pipelines.
 *
 * Stages record what they did (`trace_row*` / `trace_note`), the pipeline loop
 * calls `trace_end_cycle` once per cycle and everything is printed in one
 * fixed layout. Run statistics for the end-of-run summary are collected here
 * as well.
 */

/* Pass as pc to trace_begin_cycle when fetch is done and no PC is shown */
#define TRACE_NO_PC UINT32_MAX

typedef enum { TRACE_INORDER, TRACE_OOO } trace_mode;

/* Row ids. Each mode uses its own set, labels are picked from the mode. */
enum { TR_IF = 0, TR_ID, TR_EX, TR_MEM, TR_WB };
enum { TR_FETCH = 0, TR_ISSUE, TR_EXEC, TR_CDB };

void trace_init(trace_mode mode);
void trace_begin_cycle(uint64_t cycle, uint32_t pc);

/* Row content: a disassembled instruction (0 = idle) or free text. */
void trace_row_instr(int row, uint32_t instr);
void trace_row(int row, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
/* Annotation printed next to the row, repeated calls are joined with "; ". */
void trace_note(int row, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));

/*
 * State panel section (IQ, RS, ...). Lines are separated by '\n'. It is only
 * printed in cycles where the rendered content differs from the previous cycle.
 */
void trace_section(const char *name, const char *rendered);

void trace_end_cycle(void);

void trace_stat_retire(void);
void trace_stat_stall(const char *reason);
void trace_stat_flush(void);
void trace_summary(uint64_t cycles, const char *regs, const char *mem);

/* Appends formatted text to a NUL-terminated buffer, used by *_render(). */
void trace_appendf(char *buf, size_t cap, const char *fmt, ...)
    __attribute__((format(printf, 3, 4)));
