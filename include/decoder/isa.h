#ifndef _CHIP8_INSTRUCTION_SET_ARCHITECTURE_H_
#define _CHIP8_INSTRUCTION_SET_ARCHITECTURE_H_

#define CHIP8_OPCODE_SPECIAL           0x00
#define CHIP8_OPCODE_JMP               0x01
#define CHIP8_OPCODE_CALL              0x02
#define CHIP8_OPCODE_SE                0x03 // skip if equal
#define CHIP8_OPCODE_SNE               0x04 // skip if not equal
#define CHIP8_OPCODE_SRE               0x05 // skip if registers equal
#define CHIP8_OPCODE_SET_REGISTER      0x06 // set register
#define CHIP8_OPCODE_ADD_IMM           0x07 // add immediate
#define CHIP8_OPCODE_ALU               0x08
#define CHIP8_OPCODE_SRNE              0x09 // skip if registers not equal
#define CHIP8_OPCODE_SET_IDX           0x0A
#define CHIP8_OPCODE_JMP_REL           0x0B
#define CHIP8_OPCODE_RAND              0x0C
#define CHIP8_OPCODE_DRAW              0x0D
#define CHIP8_OPCODE_INPUT             0x0E
#define CHIP8_OPCODE_SYSTEM            0x0F

#define CHIP8_INSTRUCTION_SPECIAL_CLS  0x00E0
#define CHIP8_INSTRUCTION_SPECIAL_RET  0x00EE

#define CHIP8_ALU_OPERATION_MOV        0x00
#define CHIP8_ALU_OPERATION_OR         0x01
#define CHIP8_ALU_OPERATION_AND        0x02
#define CHIP8_ALU_OPERATION_XOR        0x03
#define CHIP8_ALU_OPERATION_ADD        0x04
#define CHIP8_ALU_OPERATION_SUB        0x05
#define CHIP8_ALU_OPERATION_SHR        0x06
#define CHIP8_ALU_OPERATION_SUBN       0x07
#define CHIP8_ALU_OPERATION_SHL        0x0E

#define CHIP8_INPUT_SP                 0x9E // skip if pressed
#define CHIP8_INPUT_SNP                0xA1 // skip if not pressed

#define CHIP8_SYSTEM_GET_DELAY_TIMER   0x07
#define CHIP8_SYSTEM_WAIT_KEY_CLICK    0x0A
#define CHIP8_SYSTEM_SET_DELAY_TIMER   0x15
#define CHIP8_SYSTEM_SET_SOUND_TIMER   0x18
#define CHIP8_SYSTEM_ADD_IDX           0x1E
#define CHIP8_SYSTEM_SET_SPRITE        0x29
#define CHIP8_SYSTEM_STORE_BCD         0x33
#define CHIP8_SYSTEM_STORE_CONTEXT     0x55
#define CHIP8_SYSTEM_LOAD_CONTEXT      0x65

#endif // _CHIP8_INSTRUCTION_SET_ARCHITECTURE_H_