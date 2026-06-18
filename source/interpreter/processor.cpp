#include <interpreter/processor.h>
#include <interpreter/machine.h>
#include <disassembler/disassembler.h>

#include <assert.h>

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

    srand( clock( ) );
}

BOOL
Chip8VirtualProcessorExecuteCycle(
    _Inout_ CHIP8_VIRTUAL_PROCESSOR* Processor,
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine
)
{
    CONST CHIP8_ENCODED_INSTRUCTION CONST* CurrentInstruction = ( CHIP8_ENCODED_INSTRUCTION* )( Machine->Memory + Processor->ProgramCounter );

    CHIP8_MACHINE_INSTRUCTION MachineInstruction = { };
    if ( Chip8DisassembleInstruction( CurrentInstruction, &MachineInstruction ) == FALSE )
    {
        return FALSE;
    }

    switch ( MachineInstruction.Mnemonic )
    {
        case CHIP8_MNEMONIC_CLS:
        {
            Chip8VirtualDisplayClear( &Machine->Display );

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_RET:
        {
            assert( Processor->CallStackSize != NULL, "Program attempted to return without any call stack" );

            Processor->CallStackSize -= 1;
            Processor->ProgramCounter = Processor->CallStack[ Processor->CallStackSize ];

            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_JMP:
        {
            Processor->ProgramCounter = MachineInstruction.Operands[ 0 ].Address;
            Processor->CycleCount += 1;

            if ( MachineInstruction.Operands[ 0 ].Flags & CHIP8_MACHINE_OPERAND_FLAG_RELATIVE_R0 )
            {
                Processor->ProgramCounter += Processor->Registers[ CHIP8_REGISTER_R0 ];
            }

        } break;

        case CHIP8_MNEMONIC_CALL:
        {
            assert( Processor->CallStackSize < ARRAYSIZE( Processor->CallStack ), "Program overran callstack" );

            Processor->CallStack[ Processor->CallStackSize ] = Processor->ProgramCounter + sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CallStackSize += 1;

            Processor->ProgramCounter = MachineInstruction.Operands[ 0 ].Address;
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SE:
        {
            BYTE DestinationValue = NULL;
            BYTE SourceValue = NULL;

            switch ( MachineInstruction.Operands[ 0 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    DestinationValue = Processor->Registers[ MachineInstruction.Operands[ 0 ].Register ];
            
                } break;
            
                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    DestinationValue = MachineInstruction.Operands[ 0 ].Immediate;
            
                } break;
            
                default: assert( "CHIP8_MNEMONIC_SE Invalid Operand[ 0 ] Type" );
            }
            
            switch ( MachineInstruction.Operands[ 1 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    SourceValue = Processor->Registers[ MachineInstruction.Operands[ 1 ].Register ];
            
                } break;
            
                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    SourceValue = MachineInstruction.Operands[ 1 ].Immediate;
            
                } break;

                default: assert( "CHIP8_MNEMONIC_SE Invalid Operand[ 1 ] Type" );
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

            switch ( MachineInstruction.Operands[ 0 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    DestinationValue = Processor->Registers[ MachineInstruction.Operands[ 0 ].Register ];
            
                } break;
            
                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    DestinationValue = MachineInstruction.Operands[ 0 ].Immediate;
            
                } break;

                default: assert( "CHIP8_MNEMONIC_SNE Invalid Operand[ 0 ] Type" );
            }
            
            switch ( MachineInstruction.Operands[ 1 ].Type )
            {
                case CHIP8_MACHINE_OPERAND_TYPE_REGISTER:
                {
                    SourceValue = Processor->Registers[ MachineInstruction.Operands[ 1 ].Register ];
            
                } break;
            
                case CHIP8_MACHINE_OPERAND_TYPE_IMMEDIATE:
                {
                    SourceValue = MachineInstruction.Operands[ 1 ].Immediate;
            
                } break;

                default: assert( "CHIP8_MNEMONIC_SNE Invalid Operand[ 1 ] Type" );
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
            CONST PCHIP8_MACHINE_OPERAND Destination = &MachineInstruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &MachineInstruction.Operands[ 1 ];

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

                        default: assert( "CHIP8_MNEMONIC_MOV::CHIP8_MACHINE_OPERAND_TYPE_REGISTER Invalid Operand[ 1 ] Type" );
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

                default: assert( "CHIP8_MNEMONIC_MOV Invalid Operand[ 0 ] Type" );
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_OR:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &MachineInstruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &MachineInstruction.Operands[ 1 ];

            Processor->Registers[ Destination->Register ] |= Processor->Registers[ Source->Register ];

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_AND:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &MachineInstruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &MachineInstruction.Operands[ 1 ];

            Processor->Registers[ Destination->Register ] &= Processor->Registers[ Source->Register ];

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_XOR:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &MachineInstruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &MachineInstruction.Operands[ 1 ];

            Processor->Registers[ Destination->Register ] ^= Processor->Registers[ Source->Register ];

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_ADD:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &MachineInstruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &MachineInstruction.Operands[ 1 ];

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
                        assert( "CHIP8_MNEMONIC_ADD Invalid Operand[ 1 ] Type" );
                    }

                } break;

                case CHIP8_MACHINE_OPERAND_TYPE_MEMORY_INDEX:
                {
                    Processor->MemoryIndex += Processor->Registers[ Source->Register ];

                } break;

                default: assert( "CHIP8_MNEMONIC_ADD Invalid Operand[ 0 ] Type" );
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SUB:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &MachineInstruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &MachineInstruction.Operands[ 1 ];

            BYTE X = Processor->Registers[ Source->Register ];
            BYTE Y = Processor->Registers[ Destination->Register ];

            Processor->Registers[ Destination->Register ] = Y - X;
            Processor->Registers[ CHIP8_REGISTER_FLAGS ] = Y >= X;

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SHR:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &MachineInstruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &MachineInstruction.Operands[ 1 ];

            BYTE ShiftValue = Processor->Registers[ Source->Register ];

            Processor->Registers[ Destination->Register ] = ShiftValue;
            Processor->Registers[ Destination->Register ] >>= 1;

            Processor->Registers[ CHIP8_REGISTER_FLAGS ] = ShiftValue & 0b00000001;

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SUBN:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &MachineInstruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &MachineInstruction.Operands[ 1 ];

            BYTE X = Processor->Registers[ Source->Register ];
            BYTE Y = Processor->Registers[ Destination->Register ];

            Processor->Registers[ Destination->Register ] = X - Y;
            Processor->Registers[ CHIP8_REGISTER_FLAGS ] = X >= Y;

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SHL:
        {
            CONST PCHIP8_MACHINE_OPERAND Destination = &MachineInstruction.Operands[ 0 ];
            CONST PCHIP8_MACHINE_OPERAND Source = &MachineInstruction.Operands[ 1 ];

            BYTE ShiftValue = Processor->Registers[ Source->Register ];

            Processor->Registers[ Destination->Register ] = ShiftValue;
            Processor->Registers[ Destination->Register ] <<= 1;

            Processor->Registers[ CHIP8_REGISTER_FLAGS ] = ShiftValue & 0b10000000;

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SKP:
        {
            BYTE Key = Processor->Registers[ MachineInstruction.Operands[ 0 ].Register ] & 0xF;
            if ( Machine->Keypad[ Key ] )
            {
                Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_SKNP:
        {
            BYTE Key = Processor->Registers[ MachineInstruction.Operands[ 0 ].Register ] & 0xF;
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

            Processor->Registers[ MachineInstruction.Operands[ 0 ].Register ] = PressedKey;

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );

        } break;

        case CHIP8_MNEMONIC_RAND:
        {
            BYTE RandomByte = rand( ) % 0x100;

            Processor->Registers[ MachineInstruction.Operands[ 0 ].Register ] = ( RandomByte & MachineInstruction.Operands[ 1 ].Immediate );

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_DRAW:
        {
            CONST BYTE X = Processor->Registers[ MachineInstruction.Operands[ 0 ].Register ];
            CONST BYTE Y = Processor->Registers[ MachineInstruction.Operands[ 1 ].Register ];
            CONST BYTE Height = MachineInstruction.Operands[ 2 ].Immediate;

            Processor->Registers[ CHIP8_REGISTER_FLAGS ] = 0;

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
            for ( CHIP8_REGISTER Register = NULL;
                                 Register <= MachineInstruction.Operands[ 0 ].Register;
                                 Register++ )
            {
                Machine->Memory[ Processor->MemoryIndex + Register ] = Processor->Registers[ Register ];
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        case CHIP8_MNEMONIC_VMLOAD:
        {
            for ( CHIP8_REGISTER Register = NULL;
                                 Register <= MachineInstruction.Operands[ 0 ].Register;
                                 Register++ )
            {
                Processor->Registers[ Register ] = Machine->Memory[ Processor->MemoryIndex + Register ];
            }

            Processor->ProgramCounter += sizeof( CHIP8_ENCODED_INSTRUCTION );
            Processor->CycleCount += 1;

        } break;

        default: assert( "Invalid Mnemonic" );
    }

    return TRUE;
}