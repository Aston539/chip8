#ifndef _CHIP8_ISA_MACHINE_IR_OPERAND_H_
#define _CHIP8_ISA_MACHINE_IR_OPERAND_H_

#include <isa/isa.h>

#define CHIP8_MACHINE_OPERAND_TYPE_INVALID                ( 0x00 )
#define CHIP8_MACHINE_OPERAND_TYPE_REGISTER               ( 0x01 )
#define CHIP8_MACHINE_OPERAND_TYPE_ADDRESS                ( 0x02 )
#define CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE              ( 0x03 )
#define CHIP8_MACHINE_OPERAND_TYPE_MEMORY_INDEX           ( 0x04 )
#define CHIP8_MACHINE_OPERAND_TYPE_DELAY_TIMER            ( 0x05 )
#define CHIP8_MACHINE_OPERAND_TYPE_SOUND_TIMER            ( 0x06 )

#define CHIP8_MAX_MACHINE_OPERANDS                        ( 0x03 )

#define CHIP8_MACHINE_OPERAND_FLAG_BINARY_CODED_DECIMAL   ( 1 << 0 )
#define CHIP8_MACHINE_OPERAND_FLAG_SPRITE_INDEX           ( 1 << 1 )
#define CHIP8_MACHINE_OPERAND_FLAG_RELATIVE_R0            ( 1 << 2 )
#define CHIP8_MACHINE_OPERAND_FLAG_MEMORY_ACCESS          ( 1 << 3 )

typedef BYTE CHIP8_MACHINE_OPERAND_TYPE;

typedef struct _CHIP8_MACHINE_OPERAND
{
    CHIP8_MACHINE_OPERAND_TYPE Type;

    //
    // value size in bits
    //
    BYTE Size;

    union
    {
        CHIP8_REGISTER Register;
        CHIP8_ADDRESS Address;

        BYTE Immediate;
    };

    BYTE Flags;

} CHIP8_MACHINE_OPERAND, * PCHIP8_MACHINE_OPERAND;

#endif // _CHIP8_ISA_MACHINE_IR_OPERAND_H_