#ifndef _CHIP8_ISA_DECODED_IR_INSTRUCTION_H_
#define _CHIP8_ISA_DECODED_IR_INSTRUCTION_H_

typedef struct _CHIP8_DECODED_INSTRUCTION
{
    union
    {
        CHIP8_ENCODED_INSTRUCTION Raw;

        struct
        {
            BYTE High;
            BYTE Low;
        };
    };

    //
    // abstraction of [Opcode:ExtendedOpcode]
    //
    CHIP8_OPCODE_KEY OpcodeKey;

    BYTE Opcode;
    BYTE ExtendedOpcode;

    CHIP8_REGISTER RegisterX;
    CHIP8_REGISTER RegisterY;

    //
    // can be 4 or 8 bits, only draw encodes it as 4 bits
    //
    BYTE Immediate;

    CHIP8_ADDRESS Address;

} CHIP8_DECODED_INSTRUCTION, * PCHIP8_DECODED_INSTRUCTION;

#endif // _CHIP8_ISA_DECODED_IR_INSTRUCTION_H_