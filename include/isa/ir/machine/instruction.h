#ifndef _CHIP8_ISA_MACHINE_IR_INSTRUCTION_H_
#define _CHIP8_ISA_MACHINE_IR_INSTRUCTION_H_

#include <isa/ir/machine/operand.h>

//
// intermediate instruction representation
// assembler<->encoder<->ENCODED/RAW<->decoder<->disassembler
//
typedef struct _CHIP8_MACHINE_INSTRUCTION
{
    CHIP8_MNEMONIC Mnemonic;

    CHIP8_MACHINE_OPERAND Operands[ CHIP8_MAX_MACHINE_OPERANDS ];
    BYTE OperandsCount;

} CHIP8_MACHINE_INSTRUCTION, * PCHIP8_MACHINE_INSTRUCTION;

#endif // _CHIP8_ISA_MACHINE_IR_INSTRUCTION_H_