#ifndef _CHIP8_ISA_MACHINE_IR_H_
#define _CHIP8_ISA_MACHINE_IR_H_

#include <isa/ir/machine/operand.h>
#include <isa/ir/machine/instruction.h>

#include <isa/ir/cfg.h>

typedef CHIP8_BASIC_BLOCK<CHIP8_MACHINE_INSTRUCTION> CHIP8_MACHINE_BASIC_BLOCK;
typedef CHIP8_FUNCTION<CHIP8_MACHINE_INSTRUCTION> CHIP8_MACHINE_FUNCTION;

#endif // _CHIP8_ISA_MACHINE_IR_H_