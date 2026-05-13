#include <interpreter/machine.h>
#include <disassembler/disassembler.h>

#include <cstdio>

STATIC
VOID
Chip8VirtualMachineLoadFontSet(
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine
)
{
    STATIC LPCSTR Chip8FontCharacters[ 16 ][ 5 ] = {
       {
           "####",
           "#  #",
           "#  #",
           "#  #",
           "####",
       },

       {
           "  # ",
           " ## ",
           "  # ",
           "  # ",
           "####",
       },

       {
           "####",
           "   #",
           "####",
           "#   ",
           "####",
       },

       {
           "####",
           "   #",
           "####",
           "   #",
           "####",
       },

       {
           "#  #",
           "#  #",
           "####",
           "   #",
           "   #",
       },

       {
           "####",
           "#   ",
           "####",
           "   #",
           "####",
       },

       {
           "####",
           "#   ",
           "####",
           "#  #",
           "####",
       },

       {
           "####",
           "   #",
           "  # ",
           " #  ",
           " #  ",
       },

       {
           "####",
           "#  #",
           "####",
           "#  #",
           "####",
       },

       {
           "####",
           "#  #",
           "####",
           "   #",
           "####",
       },

       {
           "####",
           "#  #",
           "####",
           "#  #",
           "#  #",
       },

       {
           "### ",
           "#  #",
           "### ",
           "#  #",
           "### ",
       },

       {
           "####",
           "#   ",
           "#   ",
           "#   ",
           "####",
       },

       {
           "### ",
           "#  #",
           "#  #",
           "#  #",
           "### ",
       },

       {
           "####",
           "#   ",
           "####",
           "#   ",
           "####",
       },

       {
           "####",
           "#   ",
           "####",
           "#   ",
           "#   ",
       },
    };

    for ( BYTE Character = NULL; 
               Character < 16; 
               Character++ )
    {
        for ( BYTE Row = NULL;
                   Row < 5;
                   Row++ )
        {
            BYTE Value = NULL;
            for ( BYTE Column = NULL;
                       Column < 5;
                       Column++ )
            {
                if ( Chip8FontCharacters[ Character ][ Row ][ Column ] == '#' )
                {
                    Value |= ( 1 << ( 7 - Column ) );
                }
            }

            Machine->Memory[ ( Character * 5 ) + Row ] = Value;
        }
    }
}

VOID
Chip8VirtualMachineStartup(
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine
)
{
    memset( &Machine->Keypad, NULL, sizeof( Machine->Keypad ) );
    memset( &Machine->PreviousKeypad, NULL, sizeof( Machine->PreviousKeypad ) );

    Chip8VirtualDisplayClear( &Machine->Display );

    //
    // setup default processor state
    //
    Chip8VirtualProcessorInitialise( &Machine->Processor );

    //
    // load font set into memory
    //
    Chip8VirtualMachineLoadFontSet( Machine );
}

BOOL
Chip8VirtualMachineLoadProgram(
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine,
    _In_ CONST BYTE CONST* Program,
    _In_ SIZE_T ProgramSize
)
{
    if ( ProgramSize == NULL || ProgramSize > ( 4096 - 512 ) )
    {
        return FALSE;
    }

    //
    // load program into virtual memory
    //
    memcpy( Machine->Memory + 0x200, Program, ProgramSize );

    return TRUE;
}

BOOL
Chip8VirtualMachineExecuteProgramCycle(
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine
)
{
    BOOL Success = Chip8VirtualProcessorExecuteCycle( &Machine->Processor, Machine );

    memcpy( Machine->PreviousKeypad, Machine->Keypad, sizeof( CHIP8_VIRTUAL_KEYPAD ) );

    return Success;
}

//BOOL
//Chip8ExecuteProgram(
//    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine,
//    _In_ CONST BYTE CONST* Program,
//    _In_ SIZE_T ProgramSize
//)
//{
//    if ( Program == NULL || ProgramSize > ( 4096 - 512 ) )
//    {
//        return FALSE;
//    }
//
//    //
//    // load program into virtual memory
//    //
//    memcpy( Machine->Memory + 0x200, Program, ProgramSize );
//
//    //
//    // setup processor context to execute the program
//    //
//    Machine->Processor.ProgramCounter = 0x200;
//    Machine->Processor.CycleCount = NULL;
//    Machine->Processor.DelayTimer = NULL;
//    Machine->Processor.SoundTimer = NULL;
//    Machine->Processor.MemoryIndex = NULL;
//    memset( Machine->Processor.Registers, NULL, sizeof( Machine->Processor.Registers ) );
//    memset( Machine->Processor.CallStack, NULL, sizeof( Machine->Processor.CallStack ) );
//
//    while ( TRUE )
//    {
//        Machine->Keypad[ 0x1 ] = ( GetAsyncKeyState( '1' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0x2 ] = ( GetAsyncKeyState( '2' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0x3 ] = ( GetAsyncKeyState( '3' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0xC ] = ( GetAsyncKeyState( '4' ) & 0x8000 ) != 0;
//
//        Machine->Keypad[ 0x4 ] = ( GetAsyncKeyState( 'Q' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0x5 ] = ( GetAsyncKeyState( 'W' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0x6 ] = ( GetAsyncKeyState( 'E' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0xD ] = ( GetAsyncKeyState( 'R' ) & 0x8000 ) != 0;
//
//        Machine->Keypad[ 0x7 ] = ( GetAsyncKeyState( 'A' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0x8 ] = ( GetAsyncKeyState( 'S' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0x9 ] = ( GetAsyncKeyState( 'D' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0xE ] = ( GetAsyncKeyState( 'F' ) & 0x8000 ) != 0;
//
//        Machine->Keypad[ 0xA ] = ( GetAsyncKeyState( 'Z' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0x0 ] = ( GetAsyncKeyState( 'X' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0xB ] = ( GetAsyncKeyState( 'C' ) & 0x8000 ) != 0;
//        Machine->Keypad[ 0xF ] = ( GetAsyncKeyState( 'V' ) & 0x8000 ) != 0;
//
//        system( "cls" );
//        for ( BYTE Y = NULL; Y < 32; Y++ )
//        {
//            for ( BYTE X = NULL; X < 64; X++ )
//            {
//                if ( Chip8VirtualDisplayGetPixel( &Machine->Display, X, Y ) )
//                {
//                    printf( " " );
//                }
//                else
//                {
//                    printf( "#" );
//                }
//            }
//            printf( "\n" );
//        }
//
//        Chip8VirtualProcessorExecuteCycle( &Machine->Processor, Machine );
//
//        Sleep( 16 );
//    }
//}