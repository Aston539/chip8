#ifndef _CHIP8_FUNCTION_H_
#define _CHIP8_FUNCTION_H_

#include <unordered_map>

#include "block.h"
#include "cfg.h"

typedef struct _CHIP8_FUNCTION
{
    UINT16 Address;

    CHIP8_BASIC_BLOCK** BasicBlocks;
    BYTE BasicBlocksCount;

    CHIP8_CONTROL_FLOW_GRAPH* ControlFlowGraph;

} CHIP8_FUNCTION, *PCHIP8_FUNCTION;

FORCEINLINE
CHIP8_FUNCTION*
Chip8FunctionCreate(
    VOID
)
{
    CHIP8_FUNCTION* Function = ( CHIP8_FUNCTION* )malloc( sizeof( CHIP8_FUNCTION ) );
    memset( Function, NULL, sizeof( CHIP8_FUNCTION ) );

    Function->ControlFlowGraph = Chip8ControlFlowGraphCreate( );

    return Function;
}

FORCEINLINE
VOID
Chip8FunctionCreate(
    _Inout_ CHIP8_FUNCTION** Function
)
{
    *Function = Chip8FunctionCreate( );
}

FORCEINLINE
VOID
Chip8FunctionDestroy(
    _In_ CHIP8_FUNCTION* Function
)
{
    if ( Function->BasicBlocks )
    {
        for ( BYTE BasicBlockIndex = NULL;
                   BasicBlockIndex < Function->BasicBlocksCount;
                   BasicBlockIndex++ )
        {
            Chip8BasicBlockDestroy( Function->BasicBlocks[ BasicBlockIndex ] );
        }

        if ( Function->BasicBlocks )
        {
            free( Function->BasicBlocks );
        }
    }

    if ( Function->ControlFlowGraph )
    {
        free( Function->ControlFlowGraph );
    }

    free( Function );
}

FORCEINLINE
VOID
Chip8FunctionAddBasicBlock(
    _Inout_ CHIP8_FUNCTION* Function,
    _In_ CHIP8_BASIC_BLOCK* BasicBlock
)
{
    Function->BasicBlocksCount += 1;
    Function->BasicBlocks = ( CHIP8_BASIC_BLOCK** )realloc( Function->BasicBlocks, Function->BasicBlocksCount * sizeof( CHIP8_BASIC_BLOCK* ) );

    Function->BasicBlocks[ Function->BasicBlocksCount - 1 ] = BasicBlock;
}

FORCEINLINE
VOID
Chip8FunctionRemoveBasicBlock(
    _Inout_ CHIP8_FUNCTION* Function,
    _In_ CHIP8_BASIC_BLOCK* BasicBlock
)
{
    BYTE BlockRemoveIndex = -1;
    for ( BYTE BlockIndex = NULL;
               BlockIndex < Function->BasicBlocksCount;
               BlockIndex++ )
    {
        if ( Function->BasicBlocks[ BlockIndex ] == BasicBlock )
        {
            BlockRemoveIndex = BlockIndex;

            break;
        }
    }

    if ( BlockRemoveIndex == ( BYTE )-1 )
    {
        return;
    }

    memmove( &Function->BasicBlocks[ BlockRemoveIndex ],
             &Function->BasicBlocks[ BlockRemoveIndex + 1 ],
             ( Function->BasicBlocksCount - BlockRemoveIndex - 1 ) * sizeof( CHIP8_BASIC_BLOCK* ) );

    Function->BasicBlocksCount -= 1;

    if ( Function->BasicBlocksCount == 0 )
    {
        free( Function->BasicBlocks );
        Function->BasicBlocks = NULL;
    }
    else
    {
        Function->BasicBlocks = ( CHIP8_BASIC_BLOCK** )realloc( Function->BasicBlocks, Function->BasicBlocksCount * sizeof( CHIP8_BASIC_BLOCK* ) );
    }
}

FORCEINLINE
BOOL
Chip8FunctionLookupBasicBlock(
    _In_ CONST CHIP8_FUNCTION CONST* Function,
    _In_ UINT16 BlockAddress,
    _Inout_opt_ CHIP8_BASIC_BLOCK** BasicBlock
)
{
    for ( BYTE BlockIndex = NULL;
               BlockIndex < Function->BasicBlocksCount;
               BlockIndex++ )
    {
        if ( Function->BasicBlocks[ BlockIndex ]->Address == BlockAddress )
        {
            if ( BasicBlock )
            {
                *BasicBlock = Function->BasicBlocks[ BlockIndex ];
            }

            return TRUE;
        }
    }

    return FALSE;
}

#endif // _CHIP8_FUNCTION_H_