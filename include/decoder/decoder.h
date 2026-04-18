#ifndef _CHIP8_DECODER_H_
#define _CHIP8_DECODER_H_

#include <defines.h>

#include <isa/isa.h>

typedef struct _CHIP8_DECODED_INSTRUCTION
{
    union
    {
        CHIP8_ENCODED_INSTRUCTION Raw;

        struct
        {
            BYTE High;
            BYTE Low;
        };
    };

    //
    // abstraction of [Opcode:ExtendedOpcode]
    //
    CHIP8_OPCODE_KEY OpcodeKey;

    BYTE Opcode;
    BYTE ExtendedOpcode;

    BYTE RegisterX;
    BYTE RegisterY;

    //
    // can be 4 or 8 bits, only draw encodes it as 4 bits
    //
    BYTE Immediate;

    UINT16 Address;

} CHIP8_DECODED_INSTRUCTION, * PCHIP8_DECODED_INSTRUCTION;

BOOL
Chip8DecodeInstruction(
    _In_ CONST UINT16 CONST* Instruction,
    _Inout_ CHIP8_DECODED_INSTRUCTION* DecodedInstruction
);

#endif