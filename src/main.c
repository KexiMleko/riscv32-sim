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
   #define OBJCOPY "riscv-elf-objcopy"
#endif

int load_instructions(char *file_path, uint32_t *instr_mem) {
  if (file_path[strlen(file_path) - 1] == 's') {

    char *cmd_buffer = malloc(512);
    snprintf(cmd_buffer, 512,
             "%s -march=rv32i -mabi=ilp32 -nostdlib -Ttext 0x0 %s "
             "-o output.elf%s"
             "%s -O binary -j .text output.elf output.bin",
             GCC,file_path,SH_SEP,OBJCOPY);
   printf("\nCompile and disassemble invoked via:\n      %s",cmd_buffer);
    int res = system(cmd_buffer);
    if (res != 0) {
      perror("Error compiling file");
      return 1;
    }
    file_path = "C:/Projects/riscv32-sim/output.bin";
  }
  FILE *file = fopen(file_path, "rb");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  int instr_count = fread(instr_mem, sizeof(uint32_t), 1024, file);
   for(int i=0;i<instr_count;i++){
      printf("\n%x",instr_mem[i]);
   }
  fclose(file);
  return 0;
}
int main(int argc, char *args[]) {
  uint32_t regs[32] = {0};
  uint32_t instr_mem[1024] = {0};
  uint32_t data_mem[1024] = {0};
  load_instructions(args[1], instr_mem);
}
