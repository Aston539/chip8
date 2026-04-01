#include <decoder/decoder.h>

STATIC
BOOL
Chip8IsValidRegister(
    _In_ BYTE Register
)
{
    return Register >= 0 && Register < 16;
}

STATIC
BOOL
Chip8InitializeRegisterParameter(
    _Inout_ CHIP8_DECODED_PARAMETER* Parameter,
    _In_ BYTE Register
)
{
    if ( Parameter == NULL || Chip8IsValidRegister( Register ) == FALSE )
    {
        return FALSE;
    }

    Parameter->Type = CHIP8_DECODED_PARAMETER_TYPE_REGISTER;
    Parameter->Register = Register;

    return TRUE;
}

STATIC
BOOL
Chip8InitializeImmediateParameter(
    _Inout_ CHIP8_DECODED_PARAMETER* Parameter,
    _In_ BYTE Immediate
)
{
    if ( Parameter == NULL )
    {
        return FALSE;
    }

    Parameter->Type = CHIP8_DECODED_PARAMETER_TYPE_IMMEDIATE;
    Parameter->Immediate = Immediate;

    return TRUE;
}

STATIC
BOOL
Chip8InitializeAddressParameter(
    _Inout_ CHIP8_DECODED_PARAMETER* Parameter,
    _In_ UINT16 Address
)
{
    if ( Parameter == NULL )
    {
        return FALSE;
    }

    Parameter->Type = CHIP8_DECODED_PARAMETER_TYPE_ADDRESS;
    Parameter->Address = Address;

    return TRUE;
}

STATIC
BOOL
Chip8DecodeSpecialInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    CONST UINT16 InstructionValue = Instruction->Raw.Low | ( Instruction->Raw.High >> 8 );
    switch ( InstructionValue )
    {
        case CHIP8_INSTRUCTION_SPECIAL_CLS:
        {
            Instruction->Opcode = CHIP8_DECODED_OPCODE_CLS;
            Instruction->ParametersCount = NULL;

            return TRUE;

        } break;

        case CHIP8_INSTRUCTION_SPECIAL_RET:
        {
            Instruction->Opcode = CHIP8_DECODED_OPCODE_RET;
            Instruction->ParametersCount = NULL;

            return TRUE;

        } break;

        default: break;
    }

    return FALSE;
}

STATIC
BOOL
Chip8DecodeJumpInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_JMP;
    Instruction->ParametersCount = 1;

    //
    // bits [4:16] hold the jump address
    //
    CONST UINT16 JumpAddress = ( ( ( UINT16 )( Instruction->Raw.High & 0x0F ) << 8 ) | Instruction->Raw.Low );
    if ( Chip8InitializeAddressParameter( &Instruction->Parameters[ 0 ], JumpAddress ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeCallInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_CALL;
    Instruction->ParametersCount = 1;

    //
    // bits [4:16] hold the call address
    //
    CONST UINT16 CallAddress = ( ( ( UINT16 )( Instruction->Raw.High & 0x0F ) << 8 ) | Instruction->Raw.Low );
    if ( Chip8InitializeAddressParameter( &Instruction->Parameters[ 0 ], CallAddress ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeSkipIfEqualInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_SE;
    Instruction->ParametersCount = 2;

    //
    // bits [4:8] holds the comparing register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [8:16] holds the constant to compare against
    //
    if ( Chip8InitializeImmediateParameter( &Instruction->Parameters[ 1 ], Instruction->Raw.Low ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeSkipIfNotEqualInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_SNE;
    Instruction->ParametersCount = 2;

    //
    // bits [4:8] holds the comparing register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [8:16] holds the constant to compare against
    //
    if ( Chip8InitializeImmediateParameter( &Instruction->Parameters[ 1 ], Instruction->Raw.Low ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeSkipIfRegistersEqualInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    //
    // bits [12:16] must be zero for this to be the sre instruction
    //
    if ( ( Instruction->Raw.High & 0x0F ) != 0 )
    {
        return FALSE;
    }

    Instruction->Opcode = CHIP8_DECODED_OPCODE_SRE;
    Instruction->ParametersCount = 2;

    //
    // bits [4:8] holds the comparing lefthand register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [8:16] holds the comparing righthand register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 1 ], ( Instruction->Raw.Low & 0xF0 ) >> 4 ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeSetRegisterInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_SET_REGISTER;
    Instruction->ParametersCount = 2;

    //
    // bits [4:8] holds the destination register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [8:16] holds the source immediate
    //
    if ( Chip8InitializeImmediateParameter( &Instruction->Parameters[ 1 ], Instruction->Raw.Low ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeAddImmediateInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_ADD_IMM;
    Instruction->ParametersCount = 2;

    //
    // bits [4:8] holds the destination register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [8:16] holds the source immediate
    //
    if ( Chip8InitializeImmediateParameter( &Instruction->Parameters[ 1 ], Instruction->Raw.Low ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeALUInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    CONST BYTE ExtendedOpcode = ( Instruction->Raw.Low & 0x0F ) << 4;
    switch ( ExtendedOpcode )
    {
        case CHIP8_ALU_OPERATION_MOV:  Instruction->Opcode = CHIP8_DECODED_OPCODE_MOV;  break;
        case CHIP8_ALU_OPERATION_OR:   Instruction->Opcode = CHIP8_DECODED_OPCODE_OR;   break;
        case CHIP8_ALU_OPERATION_AND:  Instruction->Opcode = CHIP8_DECODED_OPCODE_AND;  break;
        case CHIP8_ALU_OPERATION_XOR:  Instruction->Opcode = CHIP8_DECODED_OPCODE_XOR;  break;
        case CHIP8_ALU_OPERATION_ADD:  Instruction->Opcode = CHIP8_DECODED_OPCODE_ADD;  break;
        case CHIP8_ALU_OPERATION_SUB:  Instruction->Opcode = CHIP8_DECODED_OPCODE_SUB;  break;
        case CHIP8_ALU_OPERATION_SHR:  Instruction->Opcode = CHIP8_DECODED_OPCODE_SHR;  break;
        case CHIP8_ALU_OPERATION_SUBN: Instruction->Opcode = CHIP8_DECODED_OPCODE_SUBN; break;
        case CHIP8_ALU_OPERATION_SHL:  Instruction->Opcode = CHIP8_DECODED_OPCODE_SHL;  break;

        default: return FALSE;
    }

    //
    // ALU instructions only work with registers and always have 2
    // operands destination being bits [4:8] and source being bits [8:12]
    //
    Instruction->ParametersCount = 2;

    //
    // bits [4:8] holds the destination register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [8:12] holds the source register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 1 ], ( Instruction->Raw.Low & 0xF0 ) >> 4 ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeSkipIfRegistersNotEqualInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    //
    // bits [12:16] must be zero for this to be the srne instruction
    //
    if ( ( Instruction->Raw.High & 0x0F ) != 0 )
    {
        return FALSE;
    }

    Instruction->Opcode = CHIP8_DECODED_OPCODE_SRNE;
    Instruction->ParametersCount = 2;

    //
    // bits [4:8] holds the destination register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [8:12] holds the source register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 1 ], ( Instruction->Raw.Low & 0xF0 ) >> 4 ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeSetIndexInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_SET_IDX;
    Instruction->ParametersCount = 1;

    //
    // bits [4:16] holds the address
    //
    CONST UINT16 SourceAddress = ( ( UINT16 )( Instruction->Raw.High & 0x0F ) << 8 ) | Instruction->Raw.Low;
    if ( Chip8InitializeAddressParameter( &Instruction->Parameters[ 0 ], SourceAddress ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeJumpRelativeInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_JMP_REL;
    Instruction->ParametersCount = 1;

    //
    // bits [4:16] holds the jump address
    //
    CONST UINT16 JumpAddress = ( ( UINT16 )( Instruction->Raw.High & 0x0F ) << 8 ) | Instruction->Raw.Low;
    if ( Chip8InitializeAddressParameter( &Instruction->Parameters[ 0 ], JumpAddress ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeRandomInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_RAND;
    Instruction->ParametersCount = 2;

    //
    // bits [4:8] hold the register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [8:16] hold the mask
    //
    if ( Chip8InitializeImmediateParameter( &Instruction->Parameters[ 1 ], Instruction->Raw.Low ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeDrawInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    Instruction->Opcode = CHIP8_DECODED_OPCODE_DRAW;
    Instruction->ParametersCount = 3;

    //
    // bits [4:8] hold the X register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [8:12] hold the Y register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 1 ], ( Instruction->Raw.Low & 0xF0 ) >> 4 ) == FALSE )
    {
        return FALSE;
    }

    //
    // bits [12:16] hold the size immediate
    //
    if ( Chip8InitializeImmediateParameter( &Instruction->Parameters[ 2 ], Instruction->Raw.Low & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeInputInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    //
    // bits [8:16] hold the extended opcode
    //
    CONST BYTE ExtendedOpcode = Instruction->Raw.Low;
    switch ( ExtendedOpcode )
    {
        case CHIP8_INPUT_SP:  Instruction->Opcode = CHIP8_DECODED_OPCODE_SP;  break;
        case CHIP8_INPUT_SNP: Instruction->Opcode = CHIP8_DECODED_OPCODE_SNP; break;

        default: return FALSE;
    }

    Instruction->ParametersCount = 1;

    //
    // bits [4:8] hold the register which the input key is in
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
Chip8DecodeSystemInstruction(
    _Inout_ CHIP8_DECODED_INSTRUCTION* Instruction
)
{
    //
    // bits [8:16] hold the extended opcode
    //
    CONST BYTE ExtendedOpcode = Instruction->Raw.Low;
    switch ( ExtendedOpcode )
    {
        case CHIP8_SYSTEM_GET_DELAY_TIMER: Instruction->Opcode = CHIP8_DECODED_OPCODE_GET_DELAY_TIMER; break;
        case CHIP8_SYSTEM_WAIT_KEY_CLICK:  Instruction->Opcode = CHIP8_DECODED_OPCODE_WAIT_KEY_CLICK;  break;
        case CHIP8_SYSTEM_SET_DELAY_TIMER: Instruction->Opcode = CHIP8_DECODED_OPCODE_SET_DELAY_TIMER; break;
        case CHIP8_SYSTEM_SET_SOUND_TIMER: Instruction->Opcode = CHIP8_DECODED_OPCODE_SET_SOUND_TIMER; break;
        case CHIP8_SYSTEM_ADD_IDX:         Instruction->Opcode = CHIP8_DECODED_OPCODE_ADD_IDX;         break;
        case CHIP8_SYSTEM_SET_SPRITE:      Instruction->Opcode = CHIP8_DECODED_OPCODE_SET_SPRITE;      break;
        case CHIP8_SYSTEM_STORE_BCD:       Instruction->Opcode = CHIP8_DECODED_OPCODE_STORE_BCD;       break;
        case CHIP8_SYSTEM_STORE_CONTEXT:   Instruction->Opcode = CHIP8_DECODED_OPCODE_STORE_CONTEXT;   break;
        case CHIP8_SYSTEM_LOAD_CONTEXT:    Instruction->Opcode = CHIP8_DECODED_OPCODE_LOAD_CONTEXT;    break;

        default: return FALSE;
    }

    //
    // every system instruction has 1 parameter being a register
    //
    Instruction->ParametersCount = 1;

    //
    // bits [4:8] hold the register
    //
    if ( Chip8InitializeRegisterParameter( &Instruction->Parameters[ 0 ], Instruction->Raw.High & 0x0F ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

BOOL
Chip8DecodeInstruction(
    _In_ CONST BYTE CONST* Instruction,
    _Inout_ CHIP8_DECODED_INSTRUCTION* DecodedInstruction
)
{
    DecodedInstruction->Raw.High = Instruction[ 0 ];
    DecodedInstruction->Raw.Low = Instruction[ 1 ];

    //
    // base opcode is the first 4 bits 
    //
    CONST BYTE Opcode = ( DecodedInstruction->Raw.High & 0xF0 ) >> 4;
    switch ( Opcode )
    {
        case CHIP8_OPCODE_SPECIAL:      return Chip8DecodeSpecialInstruction( DecodedInstruction );
        case CHIP8_OPCODE_JMP:          return Chip8DecodeJumpInstruction( DecodedInstruction );
        case CHIP8_OPCODE_CALL:         return Chip8DecodeCallInstruction( DecodedInstruction );
        case CHIP8_OPCODE_SE:           return Chip8DecodeSkipIfEqualInstruction( DecodedInstruction );
        case CHIP8_OPCODE_SNE:          return Chip8DecodeSkipIfNotEqualInstruction( DecodedInstruction );
        case CHIP8_OPCODE_SRE:          return Chip8DecodeSkipIfRegistersEqualInstruction( DecodedInstruction );
        case CHIP8_OPCODE_SET_REGISTER: return Chip8DecodeSetRegisterInstruction( DecodedInstruction );
        case CHIP8_OPCODE_ADD_IMM:      return Chip8DecodeAddImmediateInstruction( DecodedInstruction );
        case CHIP8_OPCODE_ALU:          return Chip8DecodeALUInstruction( DecodedInstruction );
        case CHIP8_OPCODE_SRNE:         return Chip8DecodeSkipIfRegistersNotEqualInstruction( DecodedInstruction );
        case CHIP8_OPCODE_SET_IDX:      return Chip8DecodeSetIndexInstruction( DecodedInstruction );
        case CHIP8_OPCODE_JMP_REL:      return Chip8DecodeJumpRelativeInstruction( DecodedInstruction );
        case CHIP8_OPCODE_RAND:         return Chip8DecodeRandomInstruction( DecodedInstruction );
        case CHIP8_OPCODE_DRAW:         return Chip8DecodeDrawInstruction( DecodedInstruction );
        case CHIP8_OPCODE_INPUT:        return Chip8DecodeInputInstruction( DecodedInstruction );
        case CHIP8_OPCODE_SYSTEM:       return Chip8DecodeSystemInstruction( DecodedInstruction );
        default: break;
    }

    return FALSE;
}