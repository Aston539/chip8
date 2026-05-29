#include <interpreter/processor.h>
#include <interpreter/machine.h>
#include <disassembler/disassembler.h>

VOID
Chip8VirtualProcessorInitialise(
    _Inout_ CHIP8_VIRTUAL_PROCESSOR* Processor
)
{
    Processor->ProgramCounter = 0x200;
    Processor->CycleCount = NULL;
    Processor->DelayTimer = NULL;
    Processor->SoundTimer = NULL;
    Processor->MemoryIndex = NULL;
    Processor->CallStackSize = NULL;
    memset( Processor->Registers, NULL, sizeof( Processor->Registers ) );
    memset( Processor->CallStack, NULL, sizeof( Processor->CallStack ) );
}

BOOL
Chip8VirtualProcessorExecuteCycle(
    _Inout_ CHIP8_VIRTUAL_PROCESSOR* Processor,
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine
)
{
    CHIP8_MACHINE_INSTRUCTION Instruction = { };
    if ( Chip8DisassembleInstruction( ( CONST UINT16 CONST* )( Machine->Memory + Processor->ProgramCounter ), &Instruction ) == FALSE )
    {
        return FALSE;
    }

    switch ( Instruction.Mnemonic )
    {
        case CHIP8_MNEMONIC_CLS:
        {
            Chip8VirtualDisplayClear( &Machine->Display );

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_RET:
        {
            if ( Processor->CallStackSize == NULL )
            {
                __debugbreak( );
            }

            Processor->CallStackSize -= 1;
            Processor->ProgramCounter = Processor->CallStack[ Processor->CallStackSize ];

            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_JMP:
        {
            Processor->ProgramCounter = Instruction.Operands[ 0 ].Address;
            Processor->CycleCount += 1;

            if ( Instruction.Operands[ 0 ].Flags & CHIP8_MACHINE_OPERAND_FLAG_RELATIVE_R0 )
            {
                Processor->ProgramCounter += Processor->Registers[ CHIP8_REGISTER_R0 ];
            }

        } break;

        case CHIP8_MNEMONIC_CALL:
        {
            if ( Processor->CallStackSize >= ARRAYSIZE( Processor->CallStack ) )
            {
                __debugbreak( );
            }

            Processor->CallStack[ Processor->CallStackSize ] = Processor->ProgramCounter + sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CallStackSize += 1;

            Processor->ProgramCounter = Instruction.Operands[ 0 ].Address;
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SE:
        {
            BYTE DestinationValue = NULL;
            BYTE SourceValue = NULL;

            switch ( Instruction.Operands[ 0 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    DestinationValue = Processor->Registers[ Instruction.Operands[ 0 ].Register ];
            
                } break;
            
                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    DestinationValue = Instruction.Operands[ 0 ].Immediate;
            
                } break;
            
                default: __debugbreak( );
            }
            
            switch ( Instruction.Operands[ 1 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    SourceValue = Processor->Registers[ Instruction.Operands[ 1 ].Register ];
            
                } break;
            
                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    SourceValue = Instruction.Operands[ 1 ].Immediate;
            
                } break;
            
                default: __debugbreak( );
            }

            if ( SourceValue == DestinationValue )
            {
                //
                // skip the next instruction
                //
                Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SNE:
        {
            BYTE DestinationValue = NULL;
            BYTE SourceValue = NULL;

            switch ( Instruction.Operands[ 0 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    DestinationValue = Processor->Registers[ Instruction.Operands[ 0 ].Register ];
            
                } break;
            
                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    DestinationValue = Instruction.Operands[ 0 ].Immediate;
            
                } break;
            
                default: __debugbreak( );
            }
            
            switch ( Instruction.Operands[ 1 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    SourceValue = Processor->Registers[ Instruction.Operands[ 1 ].Register ];
            
                } break;
            
                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    SourceValue = Instruction.Operands[ 1 ].Immediate;
            
                } break;
            
                default: __debugbreak( );
            }

            if ( SourceValue != DestinationValue )
            {
                //
                // skip the next instruction
                //
                Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_MOV:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &Instruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &Instruction.Operands[ 1 ];

            switch ( Destination->Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    switch ( Source->Type )
                    {
                        case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:     Processor->Registers[ Destination->Register ] = Processor->Registers[ Source->Register ]; break;
                        case CHIP8_MACHINE_OPERAND_TYPE_ADDRESS:      Processor->Registers[ Destination->Register ] = Source->Address; break;
                        case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:    Processor->Registers[ Destination->Register ] = Source->Immediate; break;
                        case CHIP8_MACHINE_OPERAND_TYPE_MEMORY_INDEX: Processor->Registers[ Destination->Register ] = Processor->MemoryIndex; break;
                        case CHIP8_MACHINE_OPERAND_TYPE_DELAY_TIMER:  Processor->Registers[ Destination->Register ] = Processor->DelayTimer; break;
                        //case CHIP8_MACHINE_OPERAND_TYPE_SOUND_TIMER:  Processor->Registers[ Destination->Register ] = Processor->SoundTimer; break; <- not supported in CHIP8

                        default: __debugbreak( );
                    }

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_MEMORY_INDEX:
                {
                    if ( Destination->Flags & CHIP8_MACHINE_OPERAND_FLAG_MEMORY_ACCESS &&
                         Source->Type == CHIP8_MACHINE_OPERAND_TYPE_REGISTER &&
                         Source->Flags & CHIP8_MACHINE_OPERAND_FLAG_BINARY_CODED_DECIMAL )
                    {
                        *( BYTE* )( &Machine->Memory[ Processor->MemoryIndex + 0 ] ) = Processor->Registers[ Source->Register ] / 100;
                        *( BYTE* )( &Machine->Memory[ Processor->MemoryIndex + 1 ] ) = ( Processor->Registers[ Source->Register ] / 10 ) % 10;
                        *( BYTE* )( &Machine->Memory[ Processor->MemoryIndex + 2 ] ) = Processor->Registers[ Source->Register ] % 10;
                    }
                    else if ( Source->Type == CHIP8_MACHINE_OPERAND_TYPE_REGISTER &&
                              Source->Flags & CHIP8_MACHINE_OPERAND_FLAG_SPRITE_INDEX )
                    {
                        Processor->MemoryIndex = CHIP8_VIRTUAL_MACHINE_FONTSET_BASE + ( Processor->Registers[ Source->Register ] * 5 );
                    }
                    else
                    {
                        Processor->MemoryIndex = Source->Address;
                    }

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_DELAY_TIMER:
                {
                    Processor->DelayTimer = Processor->Registers[ Source->Register ];

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_SOUND_TIMER:
                {
                    Processor->SoundTimer = Processor->Registers[ Source->Register ];

                } break;

                default: __debugbreak( );
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_OR:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &Instruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &Instruction.Operands[ 1 ];

            Processor->Registers[ Destination->Register ] |= Processor->Registers[ Source->Register ];

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_AND:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &Instruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &Instruction.Operands[ 1 ];

            Processor->Registers[ Destination->Register ] &= Processor->Registers[ Source->Register ];

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_XOR:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &Instruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &Instruction.Operands[ 1 ];

            Processor->Registers[ Destination->Register ] ^= Processor->Registers[ Source->Register ];

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_ADD:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &Instruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &Instruction.Operands[ 1 ];

            switch ( Destination->Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    if ( Source->Type == CHIP8_MACHINE_OPERAND_TYPE_REGISTER )
                    {
                        UINT16 Result = Processor->Registers[ Destination->Register ] + Processor->Registers[ Source->Register ];

                        Processor->Registers[ Destination->Register ] = Result & 0xFF;

                        if ( Result > 0xFF )
                        {
                            Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 1;
                        }
                        else
                        {
                            Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 0;
                        }
                    }
                    else if ( Source->Type == CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE )
                    {
                        Processor->Registers[ Destination->Register ] += Source->Immediate;
                    }
                    else
                    {
                        __debugbreak( );
                    }

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_MEMORY_INDEX:
                {
                    Processor->MemoryIndex += Processor->Registers[ Source->Register ];

                } break;

                default: __debugbreak( );
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SUB:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &Instruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &Instruction.Operands[ 1 ];

            if ( Processor->Registers[ Destination->Register ] >= Processor->Registers[ Source->Register ] )
            {
                Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 1;
            }
            else
            {
                Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 0;
            }

            Processor->Registers[ Destination->Register ] = Processor->Registers[ Destination->Register ] -
                                                            Processor->Registers[ Source->Register ];

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SHR:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &Instruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &Instruction.Operands[ 1 ];

            if ( Processor->Registers[ Source->Register ] & 0b00000001 )
            {
                Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 1;
            }
            else
            {
                Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 0;
            }

            Processor->Registers[ Destination->Register ] = Processor->Registers[ Source->Register ];
            Processor->Registers[ Destination->Register ] >>= 1;

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SUBN:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &Instruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &Instruction.Operands[ 1 ];

            if ( Processor->Registers[ Source->Register ] >= Processor->Registers[ Destination->Register ] )
            {
                Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 1;
            }
            else
            {
                Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 0;
            }

            Processor->Registers[ Destination->Register ] = Processor->Registers[ Source->Register ] -
                                                            Processor->Registers[ Destination->Register ];

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SHL:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &Instruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &Instruction.Operands[ 1 ];

            if ( Processor->Registers[ Source->Register ] & 0b10000000 )
            {
                Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 1;
            }
            else
            {
                Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 0;
            }

            Processor->Registers[ Destination->Register ] = Processor->Registers[ Source->Register ];
            Processor->Registers[ Destination->Register ] <<= 1;

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SKP:
        {
            BYTE Key = Processor->Registers[ Instruction.Operands[ 0 ].Register ];
            if ( Machine->Keypad[ Key ] )
            {
                Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SKNP:
        {
            BYTE Key = Processor->Registers[ Instruction.Operands[ 0 ].Register ];
            if ( Machine->Keypad[ Key ] == FALSE )
            {
                Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_HLT:
        {
            BYTE PressedKey = 0xFF;
            for ( BYTE Key = 0; Key < 16; Key++ )
            {
                BYTE PressedNow = Machine->Keypad[ Key ];
                BYTE PressedBefore = Machine->PreviousKeypad[ Key ];

                if ( PressedNow && PressedBefore == FALSE )
                {
                    PressedKey = Key;

                    break;
                }
            }

            Processor->CycleCount += 1;

            if ( PressedKey == 0xFF )
            {
                break;
            }

            Processor->Registers[ Instruction.Operands[ 0 ].Register ] = PressedKey;

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );

        } break;

        case CHIP8_MNEMONIC_RAND:
        {
            srand( __rdtsc( ) * ( ULONG64 )& Machine );
            BYTE RandomByte = rand( ) % 0xFF;

            Processor->Registers[ Instruction.Operands[ 0 ].Register ] = ( RandomByte & Instruction.Operands[ 1 ].Immediate );

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_DRAW:
        {
            Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 0;

            CONST BYTE X = Processor->Registers[ Instruction.Operands[ 0 ].Register ];
            CONST BYTE Y = Processor->Registers[ Instruction.Operands[ 1 ].Register ];
            CONST BYTE Height = Instruction.Operands[ 2 ].Immediate;

            for ( BYTE Row = NULL; Row < Height; Row++ )
            {
                BYTE SpriteData = Machine->Memory[ Processor->MemoryIndex + Row ];

                for ( BYTE Bit = NULL; Bit < 8; Bit++ )
                {
                    BYTE Pixel = ( SpriteData >> ( 7 - Bit ) ) & 1;
                    if ( Pixel )
                    {
                        BYTE PixelPositionX = ( X + Bit ) % 64;
                        BYTE PixelPositionY = ( Y + Row ) % 32;

                        BYTE OriginalPixel = Chip8VirtualDisplayGetPixel( &Machine->Display, PixelPositionX, PixelPositionY );
                        if ( OriginalPixel )
                        {
                            Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 1;
                        }

                        Chip8VirtualDisplaySetPixel( &Machine->Display, PixelPositionX, PixelPositionY, OriginalPixel ^ 1 );
                    }
                }
            }
            
            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_VMSAVE:
        {
            for ( BYTE Register = NULL;
                  Register <= Instruction.Operands[ 0 ].Register;
                  Register++ )
            {
                Machine->Memory[ Processor->MemoryIndex + Register ] = Processor->Registers[ Register ];
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_VMLOAD:
        {
            for ( BYTE Register = NULL;
                  Register <= Instruction.Operands[ 0 ].Register;
                  Register++ )
            {
                Processor->Registers[ Register ] = Machine->Memory[ Processor->MemoryIndex + Register ];
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        default: __debugbreak( );
    }

    return TRUE;
}