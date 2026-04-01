#ifndef _CHIP8_DISASSEMBLER_H_
#define _CHIP8_DISASSEMBLER_H_

#include <defines.h>
#include <decoder/decoder.h>

#define CHIP8_DISASSEMBLED_INSTRUCTION_MAX_TEXT 128

typedef struct _CHIP8_DISASSEMBLED_INSTRUCTION
{
    CHIP8_DECODED_INSTRUCTION Decoded;
    CHAR Text[ CHIP8_DISASSEMBLED_INSTRUCTION_MAX_TEXT + 1 ];

} CHIP8_DISASSEMBLED_INSTRUCTION, *PCHIP8_DISASSEMBLED_INSTRUCTION;

BOOL
Chip8FormatInstruction(
    _In_ CONST CHIP8_DECODED_INSTRUCTION CONST* Instruction,
    _Inout_ CHAR* Buffer,
    _In_ SIZE_T BufferSize
);

BOOL
Chip8DisassembleInstruction(
    _In_ CONST BYTE* CONST Instruction,
    _Inout_ CHIP8_DISASSEMBLED_INSTRUCTION* DisassembledInstruction
);

#endif // _CHIP8_DISASSEMBLER_H_