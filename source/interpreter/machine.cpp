#include <interpreter/machine.h>
#include <disassembler/disassembler.h>

#include <cstdio>

STATIC
VOID
Chip8VirtualMachineLoadFontSet(
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine,
    _In_ UINT16 FontsetBase
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
                       Column < 4;
                       Column++ )
            {
                if ( Chip8FontCharacters[ Character ][ Row ][ Column ] == '#' )
                {
                    Value |= ( 1 << ( 7 - Column ) );
                }
            }

            Machine->Memory[ FontsetBase + ( Character * 5 ) + Row ] = Value;
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
    memset( Machine->Memory, NULL, sizeof( Machine->Memory ) );

    Chip8VirtualDisplayClear( &Machine->Display );

    //
    // setup default processor state
    //
    Chip8VirtualProcessorInitialise( &Machine->Processor );

    //
    // load font set into memory
    //
    Chip8VirtualMachineLoadFontSet( Machine, CHIP8_VIRTUAL_MACHINE_FONTSET_BASE );
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