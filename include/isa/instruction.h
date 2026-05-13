#ifndef _CHIP8_DISASSEMBLED_INSTRUCTION_H_
#define _CHIP8_DISASSEMBLED_INSTRUCTION_H_

#include <decoder/decoder.h>

#define CHIP8_MNEMONIC_NONE                       0x00
#define CHIP8_MNEMONIC_CLS                        0x01
#define CHIP8_MNEMONIC_RET                        0x02
#define CHIP8_MNEMONIC_JMP                        0x03
#define CHIP8_MNEMONIC_CALL                       0x04
#define CHIP8_MNEMONIC_SE                         0x05
#define CHIP8_MNEMONIC_SNE                        0x06
#define CHIP8_MNEMONIC_MOV                        0x07
#define CHIP8_MNEMONIC_OR                         0x08
#define CHIP8_MNEMONIC_AND                        0x09
#define CHIP8_MNEMONIC_XOR                        0x0A
#define CHIP8_MNEMONIC_ADD                        0x0B
#define CHIP8_MNEMONIC_SUB                        0x0C
#define CHIP8_MNEMONIC_SHR                        0x0D
#define CHIP8_MNEMONIC_SUBN                       0x0E
#define CHIP8_MNEMONIC_SHL                        0x0F
#define CHIP8_MNEMONIC_SKP                        0x10
#define CHIP8_MNEMONIC_SKNP                       0x11
#define CHIP8_MNEMONIC_HLT                        0x12
#define CHIP8_MNEMONIC_RAND                       0x13
#define CHIP8_MNEMONIC_DRAW                       0x14
#define CHIP8_MNEMONIC_VMSAVE                     0x15
#define CHIP8_MNEMONIC_VMLOAD                     0x16
                                                  
#define CHIP8_OPERAND_TYPE_INVALID                0x00
#define CHIP8_OPERAND_TYPE_REGISTER               0x01
#define CHIP8_OPERAND_TYPE_ADDRESS                0x02
#define CHIP8_OPERAND_TYPE_IMMEDIATE              0x03
#define CHIP8_OPERAND_TYPE_MEMORY_INDEX           0x04
#define CHIP8_OPERAND_TYPE_DELAY_TIMER            0x05
#define CHIP8_OPERAND_TYPE_SOUND_TIMER            0x06
                                                  
#define CHIP8_MAX_OPERANDS                        0x03

#define CHIP8_OPERAND_FLAG_BINARY_CODED_DECIMAL   ( 1 << 0 )
#define CHIP8_OPERAND_FLAG_SPRITE_INDEX           ( 1 << 1 )
#define CHIP8_OPERAND_FLAG_RELATIVE_R0            ( 1 << 2 )
#define CHIP8_OPERAND_FLAG_MEMORY_ACCESS          ( 1 << 3 )

typedef BYTE CHIP8_MNEMONIC;
typedef BYTE CHIP8_OPERAND_TYPE;
typedef BYTE CHIP8_REGISTER;
typedef UINT16 CHIP8_ADDRESS;

typedef struct _CHIP8_OPERAND
{
    CHIP8_OPERAND_TYPE Type;

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

} CHIP8_OPERAND, * PCHIP8_OPERAND;

//
// intermediate instruction representation
// assembler<->encoder<->ENCODED/RAW<->decoder<->disassembler
//
typedef struct _CHIP8_INSTRUCTION
{
    CHIP8_MNEMONIC Mnemonic;

    CHIP8_OPERAND Operands[ CHIP8_MAX_OPERANDS ];
    BYTE OperandsCount;

} CHIP8_INSTRUCTION, * PCHIP8_INSTRUCTION;

FORCEINLINE
CHIP8_INSTRUCTION*
Chip8CreateInstruction(
    VOID
)
{
    CHIP8_INSTRUCTION* Instruction = ( CHIP8_INSTRUCTION* )malloc( sizeof( CHIP8_INSTRUCTION ) );
    memset( Instruction, NULL, sizeof( CHIP8_INSTRUCTION ) );

    return Instruction;
}

FORCEINLINE
VOID
Chip8CreateInstruction(
    _Inout_ CHIP8_INSTRUCTION** Instruction
)
{
    *Instruction = Chip8CreateInstruction( );
}

FORCEINLINE
VOID
Chip8DestroyInstruction(
    _In_ CHIP8_INSTRUCTION* Instruction
)
{
    free( Instruction );
}

#endif