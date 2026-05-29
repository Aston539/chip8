#include <iostream>
#include <fstream>
#include <vector>

#include <assembler/assembler.h>
#include <tools/chip8-assembler/lexer.h>

STATIC
VOID
DisplayUsage(
    VOID
)
{
    fprintf( stderr, "Usage:\n" );
    fprintf( stderr, "\tchip8-assembler <filepath>\n" );
    fprintf( stderr, "\tchip8-assembler \"<statement>\" \"<statement>\"\n" );
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

int main( int ArgumentCount, char** Arguments )
{
    if ( ArgumentCount < 2 )
    {
        DisplayUsage( );

        return 1;
    }

    std::string ProgramCode = { };
    if ( IsArgumentPath( Arguments[ 1 ] ) )
    {
        std::ifstream File( Arguments[ 1 ], std::ios::in | std::ios::binary );
        if ( File.is_open( ) == FALSE )
        {
            return 2;
        }

        File.seekg( NULL, std::ios::end );
        std::streamsize FileSize = File.tellg( );

        if ( FileSize <= NULL )
        {
            return 3;
        }

        ProgramCode.resize( FileSize, '\0' );

        File.read( ProgramCode.data( ), FileSize );
    }
    else
    {
        for ( ULONG ArgumentIndex = 1;
                    ArgumentIndex < ArgumentCount;
                    ArgumentIndex++ )
        {
            ProgramCode.append( Arguments[ ArgumentIndex ] );
        }
    }

    CHIP8_LEXER Lexer = { };
    Chip8LexerInitialize( &Lexer, ProgramCode.c_str( ), ProgramCode.size( ) );

    CHIP8_LEXER_TOKEN Token = { };
    while ( Chip8LexerAdvanceToken( &Lexer, &Token ) )
    {
        if ( Token.Type == CHIP8_LEXER_TOKEN_TYPE_END )
        {
            break;
        }

        if ( Token.Type == CHIP8_LEXER_TOKEN_TYPE_NEWLINE )
        {
            printf( "\n" );
            continue;
        }

        CHAR TokenText[ 33 ] = { 0 };

        SIZE_T CopySize = Token.TextLength;
        if ( CopySize > 32 ) CopySize = 32;

        strncpy( TokenText, Token.TextStart, CopySize );

        printf( "%s\n", TokenText );
    }
}