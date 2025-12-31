#include <stdint.h>
void parse_instr(uint32_t instr){
    int8_t opcode=((int8_t)instr)&0x7F;
    switch (opcode) {
        case 0b0110011:
            break;
        case 0b0010011:
            break;
        case 0b0000011:
            break;
        case 0b0100011:
            break;
        case 0b1100011:
            break;
        case 0b1101111:
            break;
        case 0b1100111:
            break;
        case 0b0110111:
            break;
        case 0b0010111:
            break;
        case 0b1110011:
            break; 
    }
}
