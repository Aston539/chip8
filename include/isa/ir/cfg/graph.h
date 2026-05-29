#ifndef _CHIP8_ISA_CFG_IR_CONTROL_FLOW_GRAPH_H_
#define _CHIP8_ISA_CFG_IR_CONTROL_FLOW_GRAPH_H_

#include <defines.h>

#include <map>
#include <memory>

#include <isa/ir/cfg/node.h>

class CHIP8_CONTROL_FLOW_GRAPH
{
    typedef std::map<CHIP8_ADDRESS, CHIP8_CONTROL_FLOW_NODE> CHIP8_CONTROL_FLOW_NODES;

private:

    CHIP8_CONTROL_FLOW_NODES Nodes;

public:

    VOID AddNode( _In_ CHIP8_ADDRESS Address )
    {
        if ( Nodes.contains( Address ) == FALSE )
        {
            CHIP8_CONTROL_FLOW_NODE NewNode = { };
            NewNode.Address = Address;

            Nodes.insert( { Address, std::move( NewNode ) } );
        }
    }

    VOID AddEdge( _In_ CHIP8_ADDRESS Source,
                  _In_ CHIP8_ADDRESS Destination )
    {
        AddNode( Source );
        AddNode( Destination );

        Nodes.at( Source ).AddSuccessor( Destination );
        Nodes.at( Destination ).AddPredecessor( Source );
    }

    BOOL LookupNodeByAddress( _In_ CHIP8_ADDRESS Address,
                              _Inout_opt_ CHIP8_CONTROL_FLOW_NODE** ControlFlowNode )
    {
        CONST std::map<CHIP8_ADDRESS, CHIP8_CONTROL_FLOW_NODE>::iterator& NodeIterator = Nodes.find( Address );
        if ( NodeIterator != Nodes.end( ) )
        {
            if ( ControlFlowNode )
            {
                *ControlFlowNode = &NodeIterator->second;
            }

            return TRUE;
        }

        return FALSE;
    }

    BOOL LookupNodeByAddress( _In_ CHIP8_ADDRESS Address,
                              _Inout_opt_ CHIP8_CONTROL_FLOW_NODE** ControlFlowNode ) CONST
    {
        return LookupNodeByAddress( Address, ControlFlowNode );
    }

    CONST CHIP8_CONTROL_FLOW_NODES& GetNodes( ) CONST
    {
        return Nodes;
    }
};

#endif // _CHIP8_ISA_CFG_IR_CONTROL_FLOW_GRAPH_H_