#include <stdint.h>
#include <stdlib.h>
uint32_t *load_instructions(char *file_path, uint32_t *instr_mem){
   // char *cmd_buffer=malloc(512);
   //  snprintf(cmd_buffer,512,"riscv64-elf-gcc -march=rv32i -mabi=ilp32 -nostdlib -Ttext 0x0 %s -o output.elf;riscv-elf-objcopy -O binary -j .text output.elf output.bin",file_path );
   //  int res=system(cmd_buffer);
   //  if(res!=0) 
    // FILE *file=fopen(file_path, "r");
    // gets(, 0, file);

}
int main(int argc, char *args[]) {
    uint32_t regs[32]={0};
    uint32_t instr_mem[1024]={0};
    uint32_t data_mem[1024]={0};
}

