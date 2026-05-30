#ifndef _CHIP8_ENCODER_H_
#define _CHIP8_ENCODER_H_

#include <isa/isa.h>
#include <isa/ir/machine.h>

CHIP8_ENCODED_INSTRUCTION
Chip8EncodeInstruction(
    _In_ CHIP8_OPCODE_KEY OpcodeKey,
    _In_ CHIP8_REGISTER RegisterX,
    _In_ CHIP8_REGISTER RegisterY,
    _In_ BYTE Immediate,
    _In_ CHIP8_ADDRESS Address
);

CHIP8_ENCODED_INSTRUCTION
Chip8EncodeMachineInstruction(
    _In_ CONST CHIP8_MACHINE_INSTRUCTION CONST* Instruction
);

#endif // _CHIP8_ENCODER_H_