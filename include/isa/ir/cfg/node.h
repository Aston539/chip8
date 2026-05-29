#ifndef _CHIP8_ISA_CFG_IR_CONTROL_FLOW_NODE_H_
#define _CHIP8_ISA_CFG_IR_CONTROL_FLOW_NODE_H_

#include <defines.h>

#include <unordered_set>

class CHIP8_CONTROL_FLOW_NODE
{
public:

    CHIP8_ADDRESS Address;

    std::unordered_set<CHIP8_ADDRESS> Predecessors;
    std::unordered_set<CHIP8_ADDRESS> Successors;

public:

    BOOL HasPredecessor( _In_ CHIP8_ADDRESS Predecessor ) CONST
    {
        return Predecessors.find( Predecessor ) != Predecessors.end( );
    }

    BOOL HasSuccessor( _In_ CHIP8_ADDRESS Successor ) CONST
    {
        return Successors.find( Successor ) != Successors.end( );
    }

    VOID AddPredecessor( _In_ CHIP8_ADDRESS Predecessor )
    {
        Predecessors.insert( Predecessor );
    }

    VOID AddSuccessor( _In_ CHIP8_ADDRESS Successor )
    {
        Successors.insert( Successor );
    }
};

#endif // _CHIP8_ISA_CFG_IR_CONTROL_FLOW_NODE_H_