#ifndef _CHIP8_BASIC_BLOCK_H_
#define _CHIP8_BASIC_BLOCK_H_

#include <vector>

#include "instruction.h"

typedef struct _CHIP8_BASIC_BLOCK
{
    UINT16 Address;

    CHIP8_INSTRUCTION** Instructions;
    BYTE InstructionsCount;

} CHIP8_BASIC_BLOCK, *PCHIP8_BASIC_BLOCK;

FORCEINLINE
CHIP8_BASIC_BLOCK*
Chip8BasicBlockCreate(
    VOID
)
{
    CHIP8_BASIC_BLOCK* BasicBlock = ( CHIP8_BASIC_BLOCK* )malloc( sizeof( CHIP8_BASIC_BLOCK ) );
    memset( BasicBlock, NULL, sizeof( CHIP8_BASIC_BLOCK ) );

    return BasicBlock;
}

FORCEINLINE
VOID
Chip8BasicBlockCreate(
    _Inout_ CHIP8_BASIC_BLOCK** BasicBlock
)
{
    *BasicBlock = Chip8BasicBlockCreate( );
}

FORCEINLINE
VOID
Chip8BasicBlockDestroy(
    _In_ CHIP8_BASIC_BLOCK* BasicBlock
)
{
    if ( BasicBlock->Instructions )
    {
        for ( BYTE InstructionIndex = NULL;
                   InstructionIndex < BasicBlock->InstructionsCount;
                   InstructionIndex++ )
        {
            Chip8DestroyInstruction( BasicBlock->Instructions[ InstructionIndex ] );
        }

        free( BasicBlock->Instructions );
    }

    free( BasicBlock );
}

FORCEINLINE
VOID
Chip8BasicBlockAddInstruction(
    _Inout_ CHIP8_BASIC_BLOCK* BasicBlock,
    _In_ CHIP8_INSTRUCTION* Instruction
)
{
    BasicBlock->InstructionsCount += 1;
    BasicBlock->Instructions = ( CHIP8_INSTRUCTION** )realloc( BasicBlock->Instructions, BasicBlock->InstructionsCount * sizeof( CHIP8_INSTRUCTION* ) );

    BasicBlock->Instructions[ BasicBlock->InstructionsCount - 1 ] = Instruction;
}

#endif // _CHIP8_BASIC_BLOCK_H_