#ifndef _CHIP8_ISA_CFG_IR_FUNCTION_H_
#define _CHIP8_ISA_CFG_IR_FUNCTION_H_

#include <defines.h>

#include <map>

#include <isa/ir/cfg/basicblock.h>
#include <isa/ir/cfg/graph.h>

template<typename INSTRUCTION>
struct CHIP8_FUNCTION
{
    UINT16 Address;

    std::map<CHIP8_ADDRESS, CHIP8_BASIC_BLOCK<INSTRUCTION>> BasicBlocks;

    CHIP8_CONTROL_FLOW_GRAPH ControlFlowGraph;
};

#endif // _CHIP8_ISA_CFG_IR_FUNCTION_H_