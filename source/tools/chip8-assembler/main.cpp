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
    LPCSTR TestStatement = 
R"(CLS
MOV R10, 032
MOV R11, 01B
MOV INDEX, 609
DRAW R10, R11, 004
MOV R10, 03A
MOV INDEX, 60D
DRAW R10, R11, 004
MOV R13, 000
MOV R14, 000
MOV INDEX, 5F7
CALL 210
MOV R10, 016
MOV R11, 000
MOV R1, 00F
MOV R13, 001
CALL 280
MOV R3, 00F
MOV R15, 014
OR R3, R15
MOV R15, 000
MOV R2, 032
OR R2, R1
MOV R14, R15
MOV R12, 03F
CALL 290
MOV R2, R14
MOV R12, 000
CALL 290
MOV R2, R3
MOV R12, 01F
CALL 290
ADD R10, 005
MOV R13, 002
CALL 280
MOV R3, 00F
MOV R15, 014
AND R3, R15
MOV R15, 000
MOV R2, 032
AND R2, R1
MOV R14, R15
MOV R12, 002
CALL 290
MOV R2, R14
MOV R12, 000
CALL 290
MOV R2, R3
MOV R12, 004
CALL 290
ADD R11, 005
MOV R10, 000
MOV R13, 003
CALL 280
MOV R3, 00F
MOV R15, 014
XOR R3, R15
MOV R15, 000
MOV R2, 032
XOR R2, R1
MOV R14, R15
MOV R12, 03D
CALL 290
MOV R2, R14
MOV R12, 000
CALL 290
MOV R2, R3
MOV R12, 01B
CALL 290
ADD R10, 005
MOV R13, 004
CALL 280
MOV R15, 014
ADD R15, R1
MOV R4, R15
MOV R3, 00F
MOV R15, 014
ADD R3, R15
MOV R15, 0AA
MOV R2, 032
ADD R2, R1
MOV R14, R15
MOV R12, 041
CALL 290
MOV R2, R14
MOV R12, 000
CALL 290
MOV R2, R3
MOV R12, 023
CALL 290
MOV R2, R4
MOV R12, 000
CALL 290
ADD R10, 001
MOV R13, 005
CALL 280
MOV R15, 014
SUB R15, R1
MOV R4, R15
MOV R3, 014
MOV R15, 00F
SUB R3, R15
MOV R5, 00A
MOV R15, 00A
SUB R5, R15
MOV R5, R15
MOV R15, 0AA
MOV R2, 032
SUB R2, R1
SE R5, 001
)";

    CHIP8_LEXER Lexer = { };
    Chip8LexerInitialize( &Lexer, TestStatement, strlen( TestStatement ) );

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

    if ( ArgumentCount < 2 )
    {
        DisplayUsage( );

        return 1;
    }

    if ( IsArgumentPath( Arguments[ 1 ] ) )
    {

    }
    else
    {
        for ( ULONG StatementIndex = 1; 
                    StatementIndex < ArgumentCount; 
                    StatementIndex++ )
        {
            LPCSTR Statement = Arguments[ StatementIndex ];
            Chip8LexToken( Statement, NULL, NULL );
        }
    }
}