#ifndef _CHIP8_ENCODER_H_
#define _CHIP8_ENCODER_H_

#include <isa/isa.h>

UINT16
Chip8EncodeInstruction(
    _In_ CHIP8_OPCODE_KEY OpcodeKey,
    _In_ BYTE RegisterX,
    _In_ BYTE RegisterY,
    _In_ BYTE Immediate,
    _In_ UINT16 Address
);

#endif // _CHIP8_ENCODER_H_