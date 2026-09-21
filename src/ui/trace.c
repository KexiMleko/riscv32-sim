#include "trace.h"
#include "instr_disasm.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_ROWS 5
#define MAX_SECTIONS 8
#define MAX_STALL_REASONS 8
#define ROW_TEXT_LEN 96
#define ROW_NOTE_LEN 192
#define SECTION_LEN 512
#define TEXT_COL_WIDTH 26
#define HEADER_WIDTH 40

static const char *const INORDER_LABELS[] = {"IF", "ID", "EX", "MEM", "WB"};
static const char *const OOO_LABELS[] = {"FETCH", "ISSUE", "EXEC", "CDB"};

typedef struct {
  char text[ROW_TEXT_LEN];
  char note[ROW_NOTE_LEN];
} trace_row_t;

typedef struct {
  char name[8];
  char text[SECTION_LEN];    // rendered this cycle
  char printed[SECTION_LEN]; // last content that was on screen
  bool pending;
} section_t;

typedef struct {
  const char *reason;
  uint64_t count;
} stall_stat;

static struct {
  trace_mode mode;
  uint64_t cycle;
  uint32_t pc;
  trace_row_t rows[MAX_ROWS];
  section_t sections[MAX_SECTIONS];
  uint64_t retired;
  uint64_t flushes;
  stall_stat stalls[MAX_STALL_REASONS];
} tr;

static int num_rows(void) { return tr.mode == TRACE_INORDER ? 5 : 4; }

static const char *row_label(int row) {
  return tr.mode == TRACE_INORDER ? INORDER_LABELS[row] : OOO_LABELS[row];
}

// Width in terminal columns: count everything but UTF-8 continuation bytes
static int display_width(const char *s) {
  int w = 0;
  for (; *s; s++)
    if (((unsigned char)*s & 0xC0) != 0x80)
      w++;
  return w;
}

static void print_pad(int width) {
  for (int i = 0; i < width; i++)
    putchar(' ');
}

void trace_appendf(char *buf, size_t cap, const char *fmt, ...) {
  size_t len = strlen(buf);
  if (len + 1 >= cap)
    return;
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf + len, cap - len, fmt, ap);
  va_end(ap);
}

void trace_init(trace_mode mode) {
  memset(&tr, 0, sizeof(tr));
  tr.mode = mode;
}

void trace_begin_cycle(uint64_t cycle, uint32_t pc) {
  tr.cycle = cycle;
  tr.pc = pc;
  memset(tr.rows, 0, sizeof(tr.rows));
  for (int i = 0; i < MAX_SECTIONS; i++)
    tr.sections[i].pending = false;
}

void trace_row_instr(int row, uint32_t instr) {
  if (row < 0 || row >= MAX_ROWS)
    return;
  if (instr == 0) {
    tr.rows[row].text[0] = '\0';
    return;
  }
  instr_disasm(instr, tr.rows[row].text, ROW_TEXT_LEN);
}

void trace_row(int row, const char *fmt, ...) {
  if (row < 0 || row >= MAX_ROWS)
    return;
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(tr.rows[row].text, ROW_TEXT_LEN, fmt, ap);
  va_end(ap);
}

void trace_note(int row, const char *fmt, ...) {
  if (row < 0 || row >= MAX_ROWS)
    return;
  char *note = tr.rows[row].note;
  if (note[0])
    trace_appendf(note, ROW_NOTE_LEN, "; ");
  size_t len = strlen(note);
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(note + len, ROW_NOTE_LEN - len, fmt, ap);
  va_end(ap);
}

void trace_section(const char *name, const char *rendered) {
  section_t *sec = NULL;
  for (int i = 0; i < MAX_SECTIONS; i++) {
    if (tr.sections[i].name[0] == '\0' ||
        strcmp(tr.sections[i].name, name) == 0) {
      sec = &tr.sections[i];
      break;
    }
  }
  if (!sec)
    return;
  snprintf(sec->name, sizeof(sec->name), "%s", name);
  snprintf(sec->text, SECTION_LEN, "%s", rendered);
  sec->pending = true;
}

static void print_section(const section_t *sec) {
  const char *text = sec->text[0] ? sec->text : "(empty)";
  const char *line = text;
  bool first = true;
  while (*line) {
    const char *end = strchr(line, '\n');
    int len = end ? (int)(end - line) : (int)strlen(line);
    if (first)
      printf(" ── %-5s %.*s\n", sec->name, len, line);
    else
      printf("       %-5s %.*s\n", "", len, line);
    first = false;
    if (!end)
      break;
    line = end + 1;
  }
}

void trace_end_cycle(void) {
  char header[64];
  snprintf(header, sizeof(header), "═══ Cycle %" PRIu64 " ", tr.cycle);
  printf("\n%s", header);
  for (int w = display_width(header); w < HEADER_WIDTH; w++)
    printf("═");
  if (tr.pc != TRACE_NO_PC)
    printf("  PC=0x%04x", tr.pc);
  printf("\n");

  for (int r = 0; r < num_rows(); r++) {
    const trace_row_t *row = &tr.rows[r];
    const char *text = row->text[0] ? row->text : "·";
    printf(" %-5s %s", row_label(r), text);
    if (row->note[0]) {
      int pad = TEXT_COL_WIDTH - display_width(text);
      print_pad(pad < 1 ? 1 : pad);
      printf("%s", row->note);
    }
    printf("\n");
  }

  for (int i = 0; i < MAX_SECTIONS; i++) {
    section_t *sec = &tr.sections[i];
    if (!sec->pending)
      continue;
    if (strcmp(sec->text, sec->printed) != 0) {
      print_section(sec);
      memcpy(sec->printed, sec->text, SECTION_LEN);
    }
  }
}

void trace_stat_retire(void) { tr.retired++; }

void trace_stat_flush(void) { tr.flushes++; }

void trace_stat_stall(const char *reason) {
  for (int i = 0; i < MAX_STALL_REASONS; i++) {
    if (tr.stalls[i].reason == NULL || strcmp(tr.stalls[i].reason, reason) == 0) {
      tr.stalls[i].reason = reason;
      tr.stalls[i].count++;
      return;
    }
  }
}

void trace_summary(uint64_t cycles, const char *regs, const char *mem) {
  uint64_t total_stalls = 0;
  for (int i = 0; i < MAX_STALL_REASONS; i++)
    total_stalls += tr.stalls[i].count;

  printf("\n══ Run complete ");
  for (int w = 16; w < HEADER_WIDTH; w++)
    printf("═");
  printf("\n");
  printf(" cycles   %" PRIu64 "   retired %" PRIu64 "   IPC %.2f\n", cycles,
         tr.retired, cycles ? (double)tr.retired / (double)cycles : 0.0);

  printf(" stalls   %" PRIu64, total_stalls);
  int reasons = 0;
  while (reasons < MAX_STALL_REASONS && tr.stalls[reasons].reason)
    reasons++;
  for (int i = 0; i < reasons; i++) {
    printf("%s%s", i ? ", " : " (", tr.stalls[i].reason);
    if (reasons > 1)
      printf(" %" PRIu64, tr.stalls[i].count);
  }
  printf("%s   flushes %" PRIu64 "\n", reasons ? ")" : "", tr.flushes);

  printf(" regs     %s\n", regs[0] ? regs : "(all zero)");
  printf(" memory   %s\n", mem[0] ? mem : "(untouched)");
}
