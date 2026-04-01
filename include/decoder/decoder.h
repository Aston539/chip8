#ifndef _CHIP8_DECODER_H_
#define _CHIP8_DECODER_H_

#include "defines.h"
#include "isa.h"

#define CHIP8_DECODED_OPCODE_CLS               0x00
#define CHIP8_DECODED_OPCODE_RET               0x01
#define CHIP8_DECODED_OPCODE_JMP               0x02
#define CHIP8_DECODED_OPCODE_CALL              0x03
#define CHIP8_DECODED_OPCODE_SE                0x04 // skip if equal
#define CHIP8_DECODED_OPCODE_SNE               0x05 // skip if not equal
#define CHIP8_DECODED_OPCODE_SRE               0x06 // skip if registers equal
#define CHIP8_DECODED_OPCODE_SET_REGISTER      0x07 // set register
#define CHIP8_DECODED_OPCODE_ADD_IMM           0x08 // add immediate
#define CHIP8_DECODED_OPCODE_MOV               0x09
#define CHIP8_DECODED_OPCODE_OR                0x0A
#define CHIP8_DECODED_OPCODE_AND               0x0B
#define CHIP8_DECODED_OPCODE_XOR               0x0C
#define CHIP8_DECODED_OPCODE_ADD               0x0D
#define CHIP8_DECODED_OPCODE_SUB               0x0E
#define CHIP8_DECODED_OPCODE_SHR               0x0F
#define CHIP8_DECODED_OPCODE_SUBN              0x10
#define CHIP8_DECODED_OPCODE_SHL               0x11
#define CHIP8_DECODED_OPCODE_SRNE              0x12 // skip if registers not equal
#define CHIP8_DECODED_OPCODE_SET_IDX           0x13
#define CHIP8_DECODED_OPCODE_JMP_REL           0x14
#define CHIP8_DECODED_OPCODE_RAND              0x15
#define CHIP8_DECODED_OPCODE_DRAW              0x16
#define CHIP8_DECODED_OPCODE_SP                0x17
#define CHIP8_DECODED_OPCODE_SNP               0x18
#define CHIP8_DECODED_OPCODE_GET_DELAY_TIMER   0x19
#define CHIP8_DECODED_OPCODE_WAIT_KEY_CLICK    0x1A
#define CHIP8_DECODED_OPCODE_SET_DELAY_TIMER   0x1B
#define CHIP8_DECODED_OPCODE_SET_SOUND_TIMER   0x1C
#define CHIP8_DECODED_OPCODE_ADD_IDX           0x1D
#define CHIP8_DECODED_OPCODE_SET_SPRITE        0x1E
#define CHIP8_DECODED_OPCODE_STORE_BCD         0x1F
#define CHIP8_DECODED_OPCODE_STORE_CONTEXT     0x20
#define CHIP8_DECODED_OPCODE_LOAD_CONTEXT      0x21

#define CHIP8_DECODED_PARAMETER_TYPE_REGISTER    0x00
#define CHIP8_DECODED_PARAMETER_TYPE_ADDRESS     0x01
#define CHIP8_DECODED_PARAMETER_TYPE_IMMEDIATE   0x02

typedef struct _CHIP8_DECODED_PARAMETER
{
    BYTE Type;

    union
    {
        BYTE Register;
        BYTE Immediate;

        //
        // 12 bits expressed as 16
        //
        UINT16 Address;
    };

} CHIP8_DECODED_PARAMETER, * PCHIP8_DECODED_PARAMETER;

typedef struct _CHIP8_DECODED_INSTRUCTION
{
    BYTE Opcode;

    CHIP8_DECODED_PARAMETER Parameters[ 3 ];
    BYTE ParametersCount;

    union
    {
        BYTE Bytes[ 2 ];

        struct
        {
            BYTE High;
            BYTE Low;
        };

    } Raw;

} CHIP8_DECODED_INSTRUCTION, * PCHIP8_DECODED_INSTRUCTION;

BOOL
Chip8DecodeInstruction(
    _In_ CONST BYTE CONST* Instruction,
    _Inout_ CHIP8_DECODED_INSTRUCTION* DecodedInstruction
);

#endif