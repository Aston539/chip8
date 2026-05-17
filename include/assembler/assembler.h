#ifndef _CHIP8_ASSEMBLER_H_
#define _CHIP8_ASSEMBLER_H_

#include "defines.h"

#include <isa/instruction.h>

UINT16
Chip8AssembleInstruction(
    _In_ CONST CHIP8_INSTRUCTION CONST* Instruction
);

#endif // _CHIP8_ASSEMBLER_H_