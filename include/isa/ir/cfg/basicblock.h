#ifndef _CHIP8_ISA_CFG_IR_BASIC_BLOCK_H_
#define _CHIP8_ISA_CFG_IR_BASIC_BLOCK_H_

#include <defines.h>

#include <vector>

template<typename INSTRUCTION>
struct CHIP8_BASIC_BLOCK
{
    CHIP8_ADDRESS Address;

    std::vector<INSTRUCTION> Instructions;
};

#endif // _CHIP8_ISA_CFG_IR_BASIC_BLOCK_H_