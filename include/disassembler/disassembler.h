#ifndef _CHIP8_DISASSEMBLER_H_
#define _CHIP8_DISASSEMBLER_H_

#include <vector>

#include <isa/ir/machine.h>

typedef struct _CHIP8_DISASSEMBLED_PROGRAM
{
    std::vector<CHIP8_MACHINE_FUNCTION> Functions;

} CHIP8_DISASSEMBLED_PROGRAM, *PCHIP8_DISASSEMBLED_PROGRAM;

BOOL
Chip8DisassembleInstruction(
    _In_ CONST CHIP8_ENCODED_INSTRUCTION* CONST Instruction,
    _Inout_ CHIP8_MACHINE_INSTRUCTION* DisassembledInstruction
);

BOOL
Chip8DisassembleProgram(
    _In_ CONST BYTE CONST* Program,
    _In_ SIZE_T ProgramSize,
    _Inout_ CHIP8_DISASSEMBLED_PROGRAM* Disassembly
);

BOOL
Chip8FormatInstruction(
    _In_ CONST CHIP8_MACHINE_INSTRUCTION CONST* Instruction,
    _Inout_ CHAR* Buffer,
    _In_ SIZE_T BufferSize
);

#endif // _CHIP8_DISASSEMBLER_H_