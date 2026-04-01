#include <cstdio>

#include <disassembler/disassembler.h>

BOOL
Chip8FormatInstruction(
    _In_ CONST CHIP8_DECODED_INSTRUCTION CONST* Instruction,
    _Inout_ CHAR* Buffer,
    _In_ SIZE_T BufferSize
)
{
    switch ( Instruction->Opcode )
    {
        case CHIP8_DECODED_OPCODE_CLS:
        {
            snprintf( Buffer, BufferSize, "cls" );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_RET:
        {
            snprintf( Buffer, BufferSize, "ret" );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_JMP:
        {
            snprintf( Buffer, BufferSize, "jmp 0x%03X", Instruction->Parameters[ 0 ].Address );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_CALL:
        {
            snprintf( Buffer, BufferSize, "call 0x%03X", Instruction->Parameters[ 0 ].Address );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SE:
        {
            snprintf( Buffer, BufferSize, "se R%lu, 0x%02X", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Immediate );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SNE:
        {
            snprintf( Buffer, BufferSize, "sne R%lu, 0x%02X", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Immediate );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SRE:
        {
            snprintf( Buffer, BufferSize, "sne R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SET_REGISTER:
        {
            snprintf( Buffer, BufferSize, "mov R%lu, 0x%02X", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Immediate );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_ADD_IMM:
        {
            snprintf( Buffer, BufferSize, "add R%lu, 0x%02X", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Immediate );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_MOV:
        {
            snprintf( Buffer, BufferSize, "mov R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_OR:
        {
            snprintf( Buffer, BufferSize, "or R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_AND:
        {
            snprintf( Buffer, BufferSize, "and R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_XOR:
        {
            snprintf( Buffer, BufferSize, "xor R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_ADD:
        {
            snprintf( Buffer, BufferSize, "add R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SUB:
        {
            snprintf( Buffer, BufferSize, "sub R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SHR:
        {
            snprintf( Buffer, BufferSize, "shr R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SUBN:
        {
            snprintf( Buffer, BufferSize, "subn R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SHL:
        {
            snprintf( Buffer, BufferSize, "shl R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SRNE:
        {
            snprintf( Buffer, BufferSize, "sne R%lu, R%lu", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SET_IDX:
        {
            snprintf( Buffer, BufferSize, "mov <Index>, 0x%03X", Instruction->Parameters[ 0 ].Address );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_JMP_REL:
        {
            snprintf( Buffer, BufferSize, "jmp v0, 0x%03X", Instruction->Parameters[ 0 ].Address );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_RAND:
        {
            snprintf( Buffer, BufferSize, "rnd R%lu, 0x%02X", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Immediate );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_DRAW:
        {
            snprintf( Buffer, BufferSize, "draw R%lu, R%lu, 0x%02X", Instruction->Parameters[ 0 ].Register, Instruction->Parameters[ 1 ].Register, Instruction->Parameters[ 2 ].Immediate );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SP:
        {
            snprintf( Buffer, BufferSize, "sp R%lu", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SNP:
        {
            snprintf( Buffer, BufferSize, "snp R%lu", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_GET_DELAY_TIMER:
        {
            snprintf( Buffer, BufferSize, "mov R%lu, <DelayTimer>", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_WAIT_KEY_CLICK:
        {
            snprintf( Buffer, BufferSize, "mov R%lu, <KeyPress>", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SET_DELAY_TIMER:
        {
            snprintf( Buffer, BufferSize, "mov <DelayTimer>, R%lu", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SET_SOUND_TIMER:
        {
            snprintf( Buffer, BufferSize, "mov <SoundTimer>, R%lu", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_ADD_IDX:
        {
            snprintf( Buffer, BufferSize, "add <Index>, R%lu", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_SET_SPRITE:
        {
            snprintf( Buffer, BufferSize, "mov <Index::Sprite>, hex R%lu", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_STORE_BCD:
        {
            snprintf( Buffer, BufferSize, "bcd R%lu", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_STORE_CONTEXT:
        {
            snprintf( Buffer, BufferSize, "vmsave R%lu", Instruction->Parameters[ 0 ].Register );

            return TRUE;

        } break;

        case CHIP8_DECODED_OPCODE_LOAD_CONTEXT:
        {
            snprintf( Buffer, BufferSize, "vmload R%lu", Instruction->Parameters[ 0 ].Register );

        } break;

        default: break;
    }

    return FALSE;
}

BOOL
Chip8DisassembleInstruction(
    _In_ CONST BYTE* CONST Instruction,
    _Inout_ CHIP8_DISASSEMBLED_INSTRUCTION* DisassembledInstruction
)
{
    if ( Chip8DecodeInstruction( Instruction, &DisassembledInstruction->Decoded ) == FALSE )
    {
        return FALSE;
    }

    if ( Chip8FormatInstruction( &DisassembledInstruction->Decoded,
                                 DisassembledInstruction->Text,
                                 CHIP8_DISASSEMBLED_INSTRUCTION_MAX_TEXT ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}