#include <encoder/encoder.h>

UINT16
Chip8EncodeInstruction(
    _In_ CHIP8_OPCODE_KEY OpcodeKey,
    _In_ BYTE RegisterX,
    _In_ BYTE RegisterY,
    _In_ BYTE Immediate,
    _In_ UINT16 Address
)
{
    switch ( OpcodeKey )
    {
        case CHIP8_OPCODE_KEY_SPECIAL_CLS:            return ( CHIP8_OPCODE_SPECIAL << 8 ) | CHIP8_SPECIAL_OPCODE_CLS;
        case CHIP8_OPCODE_KEY_SPECIAL_RET:            return ( CHIP8_OPCODE_SPECIAL << 8 ) | CHIP8_SPECIAL_OPCODE_RET;

        case CHIP8_OPCODE_KEY_ALU_MOV:                return ( CHIP8_OPCODE_ALU << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | CHIP8_ALU_OPCODE_MOV;
        case CHIP8_OPCODE_KEY_ALU_OR:                 return ( CHIP8_OPCODE_ALU << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | CHIP8_ALU_OPCODE_OR;
        case CHIP8_OPCODE_KEY_ALU_AND:                return ( CHIP8_OPCODE_ALU << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | CHIP8_ALU_OPCODE_AND;
        case CHIP8_OPCODE_KEY_ALU_XOR:                return ( CHIP8_OPCODE_ALU << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | CHIP8_ALU_OPCODE_XOR;
        case CHIP8_OPCODE_KEY_ALU_ADD:                return ( CHIP8_OPCODE_ALU << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | CHIP8_ALU_OPCODE_ADD;
        case CHIP8_OPCODE_KEY_ALU_SUB:                return ( CHIP8_OPCODE_ALU << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | CHIP8_ALU_OPCODE_SUB;
        case CHIP8_OPCODE_KEY_ALU_SHR:                return ( CHIP8_OPCODE_ALU << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | CHIP8_ALU_OPCODE_SHR;
        case CHIP8_OPCODE_KEY_ALU_SUBN:               return ( CHIP8_OPCODE_ALU << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | CHIP8_ALU_OPCODE_SUBN;
        case CHIP8_OPCODE_KEY_ALU_SHL:                return ( CHIP8_OPCODE_ALU << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | CHIP8_ALU_OPCODE_SHL;

        case CHIP8_OPCODE_KEY_INPUT_SKP:              return ( CHIP8_OPCODE_INPUT << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_INPUT_OPCODE_SKP;
        case CHIP8_OPCODE_KEY_INPUT_SKNP:             return ( CHIP8_OPCODE_INPUT << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_INPUT_OPCODE_SKNP;

        case CHIP8_OPCODE_KEY_SYSTEM_GET_DELAY_TIMER: return ( CHIP8_OPCODE_SYSTEM << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_SYSTEM_OPCODE_GET_DELAY_TIMER;
        case CHIP8_OPCODE_KEY_SYSTEM_WAIT_KEY_CLICK:  return ( CHIP8_OPCODE_SYSTEM << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_SYSTEM_OPCODE_WAIT_KEY_CLICK;
        case CHIP8_OPCODE_KEY_SYSTEM_SET_DELAY_TIMER: return ( CHIP8_OPCODE_SYSTEM << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_SYSTEM_OPCODE_SET_DELAY_TIMER;
        case CHIP8_OPCODE_KEY_SYSTEM_SET_SOUND_TIMER: return ( CHIP8_OPCODE_SYSTEM << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_SYSTEM_OPCODE_SET_SOUND_TIMER;
        case CHIP8_OPCODE_KEY_SYSTEM_ADD_IDX:         return ( CHIP8_OPCODE_SYSTEM << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_SYSTEM_OPCODE_ADD_IDX;
        case CHIP8_OPCODE_KEY_SYSTEM_SET_SPRITE:      return ( CHIP8_OPCODE_SYSTEM << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_SYSTEM_OPCODE_SET_SPRITE;
        case CHIP8_OPCODE_KEY_SYSTEM_STORE_BCD:       return ( CHIP8_OPCODE_SYSTEM << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_SYSTEM_OPCODE_STORE_BCD;
        case CHIP8_OPCODE_KEY_SYSTEM_STORE_CONTEXT:   return ( CHIP8_OPCODE_SYSTEM << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_SYSTEM_OPCODE_STORE_CONTEXT;
        case CHIP8_OPCODE_KEY_SYSTEM_LOAD_CONTEXT:    return ( CHIP8_OPCODE_SYSTEM << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | CHIP8_SYSTEM_OPCODE_LOAD_CONTEXT;

        case CHIP8_OPCODE_KEY_JMP:                    return ( CHIP8_OPCODE_JMP          << 12 ) | ( Address & 0x0FFF );
        case CHIP8_OPCODE_KEY_CALL:                   return ( CHIP8_OPCODE_CALL         << 12 ) | ( Address & 0x0FFF );
        case CHIP8_OPCODE_KEY_SE_VX_IMM:              return ( CHIP8_OPCODE_SE           << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( Immediate & 0xFF );
        case CHIP8_OPCODE_KEY_SNE_VX_IMM:             return ( CHIP8_OPCODE_SNE          << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( Immediate & 0xFF );
        case CHIP8_OPCODE_KEY_SE_VX_VY:               return ( CHIP8_OPCODE_SRE          << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 );
        case CHIP8_OPCODE_KEY_SET_VX_IMM:             return ( CHIP8_OPCODE_SET_REGISTER << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( Immediate & 0xFF );
        case CHIP8_OPCODE_KEY_ADD_VX_IMM:             return ( CHIP8_OPCODE_ADD_IMM      << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( Immediate & 0xFF );
        case CHIP8_OPCODE_KEY_SNE_VX_VY:              return ( CHIP8_OPCODE_SRNE         << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 );
        case CHIP8_OPCODE_KEY_DRAW_VX_VY_N:           return ( CHIP8_OPCODE_DRAW         << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( ( RegisterY & 0x0F ) << 4 ) | ( Immediate & 0x0F );
        case CHIP8_OPCODE_KEY_SET_IDX:                return ( CHIP8_OPCODE_SET_IDX      << 12 ) | ( Address & 0x0FFF );
        case CHIP8_OPCODE_KEY_JMP_REL:                return ( CHIP8_OPCODE_JMP_REL      << 12 ) | ( Address & 0x0FFF );
        case CHIP8_OPCODE_KEY_RAND:                   return ( CHIP8_OPCODE_RAND         << 12 ) | ( ( RegisterX & 0x0F ) << 8 ) | ( Immediate & 0xFF );

        default: break;
    }

    return NULL;
}