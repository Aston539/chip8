#ifndef _CHIP8_DISASSEMBLER_H_
#define _CHIP8_DISASSEMBLER_H_

#include <vector>

#include <isa/function.h>
#include <isa/instruction.h>

typedef struct _CHIP8_DISASSEMBLED_PROGRAM
{
    std::vector<CHIP8_FUNCTION*> Functions;

} CHIP8_DISASSEMBLED_PROGRAM, *PCHIP8_DISASSEMBLED_PROGRAM;

FORCEINLINE
VOID
Chip8DisassembledProgramDestroy(
    _Inout_ CHIP8_DISASSEMBLED_PROGRAM* Program
)
{
    if ( Program == NULL )
    {
        return;
    }

    for ( CHIP8_FUNCTION* Function : Program->Functions )
    {
        Chip8FunctionDestroy( Function );
    }

    Program->Functions.clear( );
}

BOOL
Chip8DisassembleInstruction(
    _In_ CONST UINT16* CONST Instruction,
    _Inout_ CHIP8_INSTRUCTION* DisassembledInstruction
);

BOOL
Chip8DisassembleProgram(
    _In_ CONST BYTE CONST* Program,
    _In_ SIZE_T ProgramSize,
    _Inout_ CHIP8_DISASSEMBLED_PROGRAM* Disassembly
);

BOOL
Chip8FormatInstruction(
    _In_ CONST CHIP8_INSTRUCTION CONST* Instruction,
    _Inout_ CHAR* Buffer,
    _In_ SIZE_T BufferSize
);

#endif // _CHIP8_DISASSEMBLER_H_