#ifndef _CHIP8_DECODER_H_
#define _CHIP8_DECODER_H_

#include <defines.h>

#include <isa/isa.h>
#include <isa/ir/decoded.h>

BOOL
Chip8DecodeInstruction(
    _In_ CONST CHIP8_ENCODED_INSTRUCTION CONST* Instruction,
    _Inout_ CHIP8_DECODED_INSTRUCTION* DecodedInstruction
);

#endif