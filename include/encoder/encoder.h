#ifndef _CHIP8_ENCODER_H_
#define _CHIP8_ENCODER_H_

#include <isa/isa.h>

CHIP8_ENCODED_INSTRUCTION
Chip8EncodeInstruction(
    _In_ CHIP8_OPCODE_KEY OpcodeKey,
    _In_ CHIP8_REGISTER RegisterX,
    _In_ CHIP8_REGISTER RegisterY,
    _In_ BYTE Immediate,
    _In_ CHIP8_ADDRESS Address
);

#endif // _CHIP8_ENCODER_H_