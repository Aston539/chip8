#ifndef _CHIP8_ASSEMBLER_LEXER_H_
#define _CHIP8_ASSEMBLER_LEXER_H_

#include <defines.h>

#define CHIP8_LEXER_TOKEN_TYPE_INVALID       0
#define CHIP8_LEXER_TOKEN_TYPE_END           1

#define CHIP8_LEXER_TOKEN_TYPE_IDENTIFIER    2
#define CHIP8_LEXER_TOKEN_TYPE_NUMBER        3
#define CHIP8_LEXER_TOKEN_TYPE_REGISTER      4

#define CHIP8_LEXER_TOKEN_TYPE_COMMA         5
#define CHIP8_LEXER_TOKEN_TYPE_COLON         6
#define CHIP8_LEXER_TOKEN_TYPE_LEFT_BRACKET  7
#define CHIP8_LEXER_TOKEN_TYPE_RIGHT_BRACKET 8
#define CHIP8_LEXER_TOKEN_TYPE_NEWLINE       9

typedef BYTE CHIP8_LEXER_TOKEN_TYPE;

typedef struct _CHIP8_LEXER_TOKEN
{
    CHIP8_LEXER_TOKEN_TYPE Type;

    LPCSTR TextStart;
    SIZE_T TextLength;

} CHIP8_LEXER_TOKEN, *PCHIP8_LEXER_TOKEN;

BOOL
Chip8LexToken(
    _In_ LPCSTR Input,
    _In_ SIZE_T Length,
    _Inout_ CHIP8_LEXER_TOKEN* Token
);

typedef struct _CHIP8_LEXER
{
    LPCSTR Start;
    LPCSTR End;
    LPCSTR Cursor;

} CHIP8_LEXER, *PCHIP8_LEXER;

VOID
Chip8LexerInitialize(
    _Inout_ CHIP8_LEXER* Lexer,
    _In_ LPCSTR Start,
    _In_ SIZE_T Length
);

BOOL
Chip8LexerPeekToken(
    _In_ CONST CHIP8_LEXER CONST* Lexer,
    _Inout_ CHIP8_LEXER_TOKEN* Token
);

BOOL
Chip8LexerAdvanceToken(
    _Inout_ CHIP8_LEXER* Lexer,
    _Inout_opt_ CHIP8_LEXER_TOKEN* Token
);

#endif // _CHIP8_ASSEMBLER_LEXER_H_