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

    CHIP8_CONTROL_FLOW_NODE* LookupNodeByAddress( _In_ CHIP8_ADDRESS Address )
    {
        CONST std::map<CHIP8_ADDRESS, CHIP8_CONTROL_FLOW_NODE>::iterator& NodeIterator = Nodes.find( Address );
        if ( NodeIterator == Nodes.end( ) )
        {
            return NULL;
        }

        return &NodeIterator->second;
    }

    CONST CHIP8_CONTROL_FLOW_NODE* LookupNodeByAddress( _In_ CHIP8_ADDRESS Address ) CONST
    {
        CONST std::map<CHIP8_ADDRESS, CHIP8_CONTROL_FLOW_NODE>::const_iterator& NodeIterator = Nodes.find( Address );
        if ( NodeIterator == Nodes.end( ) )
        {
            return NULL;
        }

        return &NodeIterator->second;
    }

    CONST CHIP8_CONTROL_FLOW_NODES& GetNodes( ) CONST
    {
        return Nodes;
    }
};

#endif // _CHIP8_ISA_CFG_IR_CONTROL_FLOW_GRAPH_H_