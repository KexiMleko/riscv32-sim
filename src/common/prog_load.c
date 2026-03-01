#include "prog_load.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define SH_SEP " && "
#define GCC "riscv-none-elf-gcc"
#define OBJCOPY "riscv-none-elf-objcopy"
#else
#define SH_SEP " ; "
#define GCC "riscv64-elf-gcc"
#define OBJCOPY "riscv64-elf-objcopy"
#endif

size_t load_instructions(char *file_path, uint32_t *instr_mem) {
  if (file_path[strlen(file_path) - 1] == 's') {
    size_t cmd_len = 512;
    char cmd_buffer[cmd_len] = {};
    snprintf(cmd_buffer, cmd_len,
             "%s -march=rv32i -mabi=ilp32 -nostdlib -Ttext 0x0 %s "
             "-o output.elf%s"
             "%s -O binary -j .text output.elf output.bin",
             GCC, file_path, SH_SEP, OBJCOPY);
    printf("\nCompile and disassemble invoked via:\n      %s", cmd_buffer);
    int res = system(cmd_buffer);
    if (res != 0) {
      perror("Error compiling file");
      return 0;
    }
    file_path = "./output.bin";
  }
  FILE *file = fopen(file_path, "rb");
  if (file == NULL) {
    perror("Error opening file");
    return 0;
  }
  size_t instr_count = fread(instr_mem, sizeof(uint32_t), 1024, file);
  printf("\n\nLoaded %zu instructions into instruction memory\n\n",
         instr_count);
  fclose(file);
  return instr_count;
}
