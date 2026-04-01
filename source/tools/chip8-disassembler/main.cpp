#include <iostream>
#include <fstream>
#include <vector>

#include <disassembler/disassembler.h>

STATIC
VOID
DisplayUsage(
    VOID
)
{
    fprintf( stderr, "Usage:\n" );
    fprintf( stderr, "\tchip8-disassembler <filepath>\n" );
    fprintf( stderr, "\tchip8-disassembler <byte1> <rawinstruction1> <byte2>\n" );
}

STATIC
BOOL
IsArgumentPath(
    _In_ LPCSTR Argument
)
{
    std::ifstream File( Argument, std::ios::binary );
    if ( File.is_open( ) == FALSE )
    {
        return FALSE;
    }

    File.seekg( NULL, std::ios::end );
    std::streamsize FileSize = File.tellg( );

    if ( FileSize <= NULL )
    {
        return FALSE;
    }

    return TRUE;
}

STATIC
BOOL
IsArgumentHexByte(
    _In_ LPCSTR Argument
)
{
    if ( strlen( Argument ) != 2 )
    {
        return FALSE;
    }

    for ( ULONG I = NULL; I < 2; I++ )
    {
        if ( isxdigit( Argument[ I ] ) == FALSE )
        {
            return FALSE;
        }
    }

    return TRUE;
}

STATIC
BOOL
IsArgumentRawInstruction(
    _In_ LPCSTR Argument
)
{
    if ( strlen( Argument ) != 4 )
    {
        return FALSE;
    }

    for ( ULONG I = NULL; I < 4; I++ )
    {
        if ( isxdigit( Argument[ I ] ) == FALSE )
        {
            return FALSE;
        }
    }

    return TRUE;
}

STATIC
VOID
DisplayInstructionsDisassembly(
    _In_ CONST BYTE CONST* Code,
    _In_ SIZE_T CodeSize
)
{
    CONSTEXPR ULONG32 CHIP8_CODE_BASE_ADDRESS = 0x200;

    for ( ULONG32 CodeOffset = NULL;
                  CodeOffset < CodeSize;
                  CodeOffset += sizeof( WORD ) )
    {
        CHAR DisassemblyFormat[ 256 + 1 ] = { };

        snprintf( DisassemblyFormat, 256, "[ %-14p ]\t%02X %02X", CHIP8_CODE_BASE_ADDRESS + CodeOffset, Code[ CodeOffset ], Code[ CodeOffset + 1 ] );

        CHIP8_DISASSEMBLED_INSTRUCTION Instruction = { };
        if ( Chip8DisassembleInstruction( Code + CodeOffset, &Instruction ) )
        {
            snprintf( DisassemblyFormat, 256, "%s\t%-32s", DisassemblyFormat, Instruction.Text );
        }
        else
        {
            snprintf( DisassemblyFormat, 256, "%s\t%-32s", DisassemblyFormat, "???" );
        }

        printf( "%s\n", DisassemblyFormat );
    }
}

int main( int ArgumentCount, char** Arguments )
{
    if ( ArgumentCount < 2 )
    {
        DisplayUsage( );

        return 1;
    }

    std::vector<BYTE> Code = { };
    if ( ArgumentCount == 2 && IsArgumentPath( Arguments[ 1 ] ) )
    {
        std::ifstream File( Arguments[ 1 ], std::ios::in | std::ios::binary );
        if ( File.is_open( ) == FALSE )
        {
            return 1;
        }

        File.seekg( NULL, std::ios::end );
        std::streamsize FileSize = File.tellg( );
        File.seekg( NULL, std::ios::beg );

        Code.resize( FileSize );
        File.read( ( CHAR* )Code.data( ), FileSize );
    }
    else
    {
        for ( ULONG ArgumentIndex = 1; 
                    ArgumentIndex < ArgumentCount; 
                    ArgumentIndex++ )
        {
            LPCSTR Argument = Arguments[ ArgumentIndex ];
            if ( IsArgumentHexByte( Argument ) )
            {
                Code.push_back( strtoul( Argument, NULL, 16 ) );
            }
            else if ( IsArgumentRawInstruction( Argument ) )
            {
                UINT16 RawInstruction = strtoul( Argument, NULL, 16 );

                BYTE High = ( BYTE )( ( RawInstruction >> 8 ) & 0xFF );
                BYTE Low  = ( BYTE )( RawInstruction & 0xFF );

                Code.push_back( High );
                Code.push_back( Low );
            }
            else
            {
                system( "cls" );

                DisplayUsage( );

                return 1;
            }
        }
    }

    DisplayInstructionsDisassembly( Code.data( ), Code.size( ) );

    return 0;
}