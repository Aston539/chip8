#include <assembler/assembler.h>
#include <encoder/encoder.h>
#include <disassembler/disassembler.h>

CHIP8_ENCODED_INSTRUCTION
Chip8AssembleInstruction(
    _In_ CONST CHIP8_MACHINE_INSTRUCTION CONST* Instruction
)
{
    CHIP8_OPCODE_KEY OpcodeKey = NULL;
    CHIP8_REGISTER RegisterX = NULL;
    CHIP8_REGISTER RegisterY = NULL;
    BYTE Immediate = NULL;
    CHIP8_ADDRESS Address = NULL;

    switch ( Instruction->Mnemonic )
    {
        case CHIP8_MNEMONIC_CLS:
        {
            OpcodeKey = CHIP8_OPCODE_KEY_SPECIAL_CLS;

        } break;

        case CHIP8_MNEMONIC_RET:
        {
            OpcodeKey = CHIP8_OPCODE_KEY_SPECIAL_RET;

        } break;

        case CHIP8_MNEMONIC_JMP:
        {
            if ( Instruction->OperandsCount != 1 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_ADDRESS )
            {
                return NULL;
            }

            if ( Instruction->Operands[ 0 ].Flags & CHIP8_MACHINE_OPERAND_FLAG_RELATIVE_R0 )
            {
                OpcodeKey = CHIP8_OPCODE_KEY_JMP_REL;
            }
            else
            {
                OpcodeKey = CHIP8_OPCODE_KEY_JMP;
            }

            Address = Instruction->Operands[ 0 ].Address;

        } break;

        case CHIP8_MNEMONIC_CALL:
        {
            if ( Instruction->OperandsCount != 1 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_ADDRESS )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_CALL;
            Address = Instruction->Operands[ 0 ].Address;

        } break;

        case CHIP8_MNEMONIC_SE:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            RegisterX = Instruction->Operands[ 0 ].Register;

            switch ( Instruction->Operands[ 1 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    OpcodeKey = CHIP8_OPCODE_KEY_SE_VX_VY;
                    RegisterY = Instruction->Operands[ 1 ].Register;

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    OpcodeKey = CHIP8_OPCODE_KEY_SE_VX_IMM;
                    Immediate = Instruction->Operands[ 1 ].Immediate;

                } break;

                default: return NULL;
            }

        } break;

        case CHIP8_MNEMONIC_SNE:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            RegisterX = Instruction->Operands[ 0 ].Register;

            switch ( Instruction->Operands[ 1 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    OpcodeKey = CHIP8_OPCODE_KEY_SNE_VX_VY;
                    RegisterY = Instruction->Operands[ 1 ].Register;

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    OpcodeKey = CHIP8_OPCODE_KEY_SNE_VX_IMM;
                    Immediate = Instruction->Operands[ 1 ].Immediate;

                } break;

                default: return NULL;
            }

        } break;

        case CHIP8_MNEMONIC_MOV:
        {
            if ( Instruction->OperandsCount != 2 )
            {
                return NULL;
            }

            //
            // opcode key lookup
            //
            switch ( Instruction->Operands[ 0 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    switch ( Instruction->Operands[ 1 ].Type )
                    {
                        case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:    OpcodeKey = CHIP8_OPCODE_KEY_ALU_MOV; break;
                        case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:   OpcodeKey = CHIP8_OPCODE_KEY_SET_VX_IMM; break;
                        case CHIP8_MACHINE_OPERAND_TYPE_DELAY_TIMER: OpcodeKey = CHIP8_OPCODE_KEY_SYSTEM_GET_DELAY_TIMER; break;

                        default: return NULL;
                    }

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_MEMORY_INDEX:
                {
                    if ( Instruction->Operands[ 0 ].Flags & CHIP8_MACHINE_OPERAND_FLAG_MEMORY_ACCESS &&
                         Instruction->Operands[ 1 ].Type == CHIP8_MACHINE_OPERAND_TYPE_REGISTER &&
                         Instruction->Operands[ 1 ].Flags & CHIP8_MACHINE_OPERAND_FLAG_BINARY_CODED_DECIMAL )
                    {
                        OpcodeKey = CHIP8_OPCODE_KEY_SYSTEM_STORE_BCD;
                    }
                    else if ( Instruction->Operands[ 1 ].Type == CHIP8_MACHINE_OPERAND_TYPE_REGISTER &&
                              Instruction->Operands[ 1 ].Flags & CHIP8_MACHINE_OPERAND_FLAG_SPRITE_INDEX )
                    {
                        OpcodeKey = CHIP8_OPCODE_KEY_SYSTEM_SET_SPRITE;
                    }
                    else if ( Instruction->Operands[ 1 ].Type == CHIP8_MACHINE_OPERAND_TYPE_ADDRESS )
                    {
                        OpcodeKey = CHIP8_OPCODE_KEY_SET_IDX;
                    }
                    else
                    {
                       return NULL;
                    }

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_DELAY_TIMER:  OpcodeKey = CHIP8_OPCODE_KEY_SYSTEM_SET_DELAY_TIMER; break;
                case CHIP8_MACHINE_OPERAND_TYPE_SOUND_TIMER:  OpcodeKey = CHIP8_OPCODE_KEY_SYSTEM_SET_SOUND_TIMER; break;

                default: return NULL;
            }

            switch ( OpcodeKey )
            {
                //
                // register, register
                //
                case CHIP8_OPCODE_KEY_ALU_MOV:
                {
                    RegisterX = Instruction->Operands[ 0 ].Register;
                    RegisterY = Instruction->Operands[ 1 ].Register;

                } break;

                //
                // register, immediate
                //
                case CHIP8_OPCODE_KEY_SET_VX_IMM:
                {
                    RegisterX = Instruction->Operands[ 0 ].Register;
                    Immediate = Instruction->Operands[ 1 ].Immediate;

                } break;

                //
                // pseudo, register
                //
                case CHIP8_OPCODE_KEY_SYSTEM_STORE_BCD:
                case CHIP8_OPCODE_KEY_SYSTEM_SET_SPRITE:
                case CHIP8_OPCODE_KEY_SYSTEM_SET_DELAY_TIMER:
                case CHIP8_OPCODE_KEY_SYSTEM_SET_SOUND_TIMER:
                {
                    RegisterX = Instruction->Operands[ 1 ].Register;

                } break;

                //
                // register, pseudo
                //
                case CHIP8_OPCODE_KEY_SYSTEM_GET_DELAY_TIMER:
                {
                    RegisterX = Instruction->Operands[ 0 ].Register;

                } break;

                //
                // pseudo, address
                //
                case CHIP8_OPCODE_KEY_SET_IDX:
                {
                    Address = Instruction->Operands[ 1 ].Address;

                } break;

                default: return NULL;
            }

        } break;

        case CHIP8_MNEMONIC_OR:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_ALU_OR;
            RegisterX = Instruction->Operands[ 0 ].Register;
            RegisterY = Instruction->Operands[ 1 ].Register;

        } break;

        case CHIP8_MNEMONIC_AND:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_ALU_AND;
            RegisterX = Instruction->Operands[ 0 ].Register;
            RegisterY = Instruction->Operands[ 1 ].Register;

        } break;

        case CHIP8_MNEMONIC_XOR:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_ALU_XOR;
            RegisterX = Instruction->Operands[ 0 ].Register;
            RegisterY = Instruction->Operands[ 1 ].Register;

        } break;

        case CHIP8_MNEMONIC_ADD:
        {
            if ( Instruction->OperandsCount != 2 )
            {
                return NULL;
            }

            switch ( Instruction->Operands[ 0 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    switch ( Instruction->Operands[ 1 ].Type )
                    {
                        case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                        {
                            OpcodeKey = CHIP8_OPCODE_KEY_ALU_ADD;
                            RegisterX = Instruction->Operands[ 0 ].Register;
                            RegisterY = Instruction->Operands[ 1 ].Register;

                        } break;

                        case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                        {
                            OpcodeKey = CHIP8_OPCODE_KEY_ADD_VX_IMM;
                            RegisterX = Instruction->Operands[ 0 ].Register;
                            Immediate = Instruction->Operands[ 1 ].Immediate;

                        } break;

                        default: return NULL;
                    }

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_MEMORY_INDEX:
                {
                    if ( Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
                    {
                        return NULL;
                    }

                    OpcodeKey = CHIP8_OPCODE_KEY_SYSTEM_ADD_IDX;
                    RegisterX = Instruction->Operands[ 1 ].Register;

                } break;

                default: return NULL;
            }

        } break;

        case CHIP8_MNEMONIC_SUB:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_ALU_SUB;
            RegisterX = Instruction->Operands[ 0 ].Register;
            RegisterY = Instruction->Operands[ 1 ].Register;

        } break;

        case CHIP8_MNEMONIC_SHR:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_ALU_SHR;
            RegisterX = Instruction->Operands[ 0 ].Register;
            RegisterY = Instruction->Operands[ 1 ].Register;

        } break;

        case CHIP8_MNEMONIC_SUBN:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_ALU_SUBN;
            RegisterX = Instruction->Operands[ 0 ].Register;
            RegisterY = Instruction->Operands[ 1 ].Register;

        } break;

        case CHIP8_MNEMONIC_SHL:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_ALU_SHL;
            RegisterX = Instruction->Operands[ 0 ].Register;
            RegisterY = Instruction->Operands[ 1 ].Register;

        } break;

        case CHIP8_MNEMONIC_SKP:
        {
            if ( Instruction->OperandsCount != 1 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_INPUT_SKP;
            RegisterX = Instruction->Operands[ 0 ].Register;

        } break;

        case CHIP8_MNEMONIC_SKNP:
        {
            if ( Instruction->OperandsCount != 1 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_INPUT_SKNP;
            RegisterX = Instruction->Operands[ 0 ].Register;

        } break;

        case CHIP8_MNEMONIC_HLT:
        {
            if ( Instruction->OperandsCount != 1 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_SYSTEM_WAIT_KEY_CLICK;
            RegisterX = Instruction->Operands[ 0 ].Register;

        } break;

        case CHIP8_MNEMONIC_RAND:
        {
            if ( Instruction->OperandsCount != 2 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_RAND;
            RegisterX = Instruction->Operands[ 0 ].Register;
            Immediate = Instruction->Operands[ 1 ].Immediate;

        } break;

        case CHIP8_MNEMONIC_DRAW:
        {
            if ( Instruction->OperandsCount != 3 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 1 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER ||
                 Instruction->Operands[ 2 ].Type != CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_DRAW_VX_VY_N;
            RegisterX = Instruction->Operands[ 0 ].Register;
            RegisterY = Instruction->Operands[ 1 ].Register;
            Immediate = Instruction->Operands[ 2 ].Immediate;

        } break;

        case CHIP8_MNEMONIC_VMSAVE:
        {
            if ( Instruction->OperandsCount != 1 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_SYSTEM_STORE_CONTEXT;
            RegisterX = Instruction->Operands[ 0 ].Register;

        } break;

        case CHIP8_MNEMONIC_VMLOAD:
        {
            if ( Instruction->OperandsCount != 1 ||
                 Instruction->Operands[ 0 ].Type != CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
            {
                return NULL;
            }

            OpcodeKey = CHIP8_OPCODE_KEY_SYSTEM_LOAD_CONTEXT;
            RegisterX = Instruction->Operands[ 0 ].Register;

        } break;

        default: __debugbreak( );
    }

    return Chip8EncodeInstruction( OpcodeKey, RegisterX, RegisterY, Immediate, Address );
}