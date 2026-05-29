#ifndef _CHIP8_ISA_CFG_IR_CONTROL_FLOW_GRAPH_H_
#define _CHIP8_ISA_CFG_IR_CONTROL_FLOW_GRAPH_H_

#include <defines.h>

#include <map>
#include <memory>

#include <isa/ir/cfg/node.h>

class CHIP8_CONTROL_FLOW_GRAPH
{
public:

    std::map<CHIP8_ADDRESS, CHIP8_CONTROL_FLOW_NODE> Nodes;

public:

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

};

#endif // _CHIP8_ISA_CFG_IR_CONTROL_FLOW_GRAPH_H_