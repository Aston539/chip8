#include <decoder/decoder.h>

STATIC
BOOL
Chip8IsValidRegister(
    _In_ BYTE Register
)
{
    return Register >= 0 && Register < 16;
}

BOOL
Chip8DecodeInstruction(
    _In_ CONST UINT16 CONST* Instruction,
    _Inout_ CHIP8_DECODED_INSTRUCTION* DecodedInstruction
)
{
    DecodedInstruction->High = *( BYTE* )Instruction;
    DecodedInstruction->Low = *( ( BYTE* )Instruction + 1 );

    //
    // opcode is in bits [0:4]
    //
    DecodedInstruction->Opcode = ( DecodedInstruction->High & 0xF0 ) >> 4;

    switch ( DecodedInstruction->Opcode )
    {
        //
        // extended opcode
        //
        case CHIP8_OPCODE_SPECIAL:
        {
            DecodedInstruction->ExtendedOpcode = DecodedInstruction->Low;
            if ( DecodedInstruction->ExtendedOpcode != CHIP8_SPECIAL_OPCODE_CLS &&
                 DecodedInstruction->ExtendedOpcode != CHIP8_SPECIAL_OPCODE_RET )
            {
                return FALSE;
            }

            DecodedInstruction->OpcodeKey = ( ( DecodedInstruction->Opcode << 8 ) | DecodedInstruction->ExtendedOpcode );

            return TRUE;

        } break;

        //
        // address
        //
        case CHIP8_OPCODE_JMP:
        case CHIP8_OPCODE_CALL:
        case CHIP8_OPCODE_SET_IDX:
        case CHIP8_OPCODE_JMP_REL:
        {
            DecodedInstruction->Address = ( ( ( UINT16 )( DecodedInstruction->High & 0x0F ) << 8 ) | DecodedInstruction->Low );
            DecodedInstruction->OpcodeKey = ( DecodedInstruction->Opcode << 12 );

            return TRUE;

        } break;

        //
        // register, 8 bit immediate
        //
        case CHIP8_OPCODE_SE:
        case CHIP8_OPCODE_SNE:
        case CHIP8_OPCODE_ADD_IMM:
        case CHIP8_OPCODE_SET_REGISTER:
        case CHIP8_OPCODE_RAND:
        {
            DecodedInstruction->RegisterX = DecodedInstruction->High & 0x0F;
            DecodedInstruction->Immediate = DecodedInstruction->Low;

            if ( Chip8IsValidRegister( DecodedInstruction->RegisterX ) == FALSE )
            {
                return FALSE;
            }

            DecodedInstruction->OpcodeKey = ( DecodedInstruction->Opcode << 12 );

            return TRUE;

        } break;

        //
        // register, register, extended opcode
        //
        case CHIP8_OPCODE_ALU:
        {
            DecodedInstruction->ExtendedOpcode = DecodedInstruction->Low & 0x0F;

            switch ( DecodedInstruction->ExtendedOpcode )
            {
                case CHIP8_ALU_OPCODE_MOV:
                case CHIP8_ALU_OPCODE_OR:
                case CHIP8_ALU_OPCODE_AND:
                case CHIP8_ALU_OPCODE_XOR:
                case CHIP8_ALU_OPCODE_ADD:
                case CHIP8_ALU_OPCODE_SUB:
                case CHIP8_ALU_OPCODE_SHR:
                case CHIP8_ALU_OPCODE_SUBN:
                case CHIP8_ALU_OPCODE_SHL:
                    break;

                default: return FALSE;
            }

            DecodedInstruction->RegisterX = DecodedInstruction->High & 0x0F;
            DecodedInstruction->RegisterY = ( DecodedInstruction->Low & 0xF0 ) >> 4;

            if ( Chip8IsValidRegister( DecodedInstruction->RegisterX ) == FALSE ||
                 Chip8IsValidRegister( DecodedInstruction->RegisterY ) == FALSE )
            {
                return FALSE;
            }

            DecodedInstruction->OpcodeKey = ( ( DecodedInstruction->Opcode << 12 ) | DecodedInstruction->ExtendedOpcode );

            return TRUE;

        } break;

        //
        // register, register
        //
        case CHIP8_OPCODE_SRE:
        case CHIP8_OPCODE_SRNE:
        {
            DecodedInstruction->RegisterX = DecodedInstruction->High & 0x0F;
            DecodedInstruction->RegisterY = ( DecodedInstruction->Low & 0xF0 ) >> 4;

            if ( Chip8IsValidRegister( DecodedInstruction->RegisterX ) == FALSE || 
                 Chip8IsValidRegister( DecodedInstruction->RegisterY ) == FALSE )
            {
                return FALSE;
            }

            DecodedInstruction->OpcodeKey = ( DecodedInstruction->Opcode << 12 );

            return TRUE;

        } break;

        //
        // register, register, immediate
        //
        case CHIP8_OPCODE_DRAW:
        {
            DecodedInstruction->RegisterX = DecodedInstruction->High & 0x0F;
            DecodedInstruction->RegisterY = ( DecodedInstruction->Low & 0xF0 ) >> 4;
            DecodedInstruction->Immediate = DecodedInstruction->Low & 0x0F;

            if ( Chip8IsValidRegister( DecodedInstruction->RegisterX ) == FALSE ||
                 Chip8IsValidRegister( DecodedInstruction->RegisterY ) == FALSE )
            {
                return FALSE;
            }

            DecodedInstruction->OpcodeKey = ( DecodedInstruction->Opcode << 12 );

            return TRUE;

        } break;

        //
        // register, extended opcode
        //
        case CHIP8_OPCODE_INPUT:
        case CHIP8_OPCODE_SYSTEM:
        {
            DecodedInstruction->ExtendedOpcode = DecodedInstruction->Low;

            DecodedInstruction->RegisterX = DecodedInstruction->High & 0x0F;

            if ( Chip8IsValidRegister( DecodedInstruction->RegisterX ) == FALSE  )
            {
                return FALSE;
            }

            DecodedInstruction->OpcodeKey = ( ( DecodedInstruction->Opcode << 12 ) | DecodedInstruction->ExtendedOpcode );

            return TRUE;

        } break; 

        default: break;
    }

    return FALSE;
}