#ifndef _CHIP8_OPCODE_H_
#define _CHIP8_OPCODE_H_

typedef unsigned short CHIP8_OPCODE;

//
// control flow
//
#define CHIP8_OPCODE_SYSTEM_CALL         0x0000
#define CHIP8_OPCODE_RETURN              0x00EE
#define CHIP8_OPCODE_JUMP                0x1000
#define CHIP8_OPCODE_CALL                0x2000
#define CHIP8_OPCODE_JUMP_RELATIVE       0xB000

//
// display
//
#define CHIP8_OPCODE_CLEAR_SCREEN        0x00E0
#define CHIP8_OPCODE_DISPLAY             0xD000

//
// conditional
//
#define CHIP8_OPCODE_EQUAL               0x3000
#define CHIP8_OPCODE_NOT_EQUAL           0x4000
#define CHIP8_OPCODE_EQUAL_REGISTER      0x5000
#define CHIP8_OPCODE_NOT_EQUAL_REGISTER  0x9000

//
// arithmetic
//

#endif