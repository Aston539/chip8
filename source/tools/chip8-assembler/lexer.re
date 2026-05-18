#include <tools/chip8-assembler/lexer.h>

#define YYCTYPE CHAR
#define YYCURSOR Cursor
#define YYMARKER Marker
#define YYLIMIT Limit

BOOL
Chip8LexToken(
    _In_ LPCSTR Input,
    _In_ SIZE_T Length,
    _Inout_ CHIP8_LEXER_TOKEN* Token
)
{
	LPCSTR Cursor = Input;
    LPCSTR Marker = NULL;
	LPCSTR Limit = Input + Length;

next:

	if ( Cursor >= Limit )
	{
		Token->Type = CHIP8_LEXER_TOKEN_TYPE_END;
		Token->TextStart = Cursor;
		Token->TextLength = NULL;

		return TRUE;
	}

	LPCSTR TokenStart = Cursor;

    Token->TextStart = TokenStart;
    Token->TextLength = NULL;
    Token->Type = CHIP8_LEXER_TOKEN_TYPE_INVALID;

    /*!re2c

		re2c:yyfill:enable = 0;

        "," {
            Token->Type = CHIP8_LEXER_TOKEN_TYPE_COMMA;
            Token->TextLength = Cursor - TokenStart;
            return TRUE;
        }

        ":" {
            Token->Type = CHIP8_LEXER_TOKEN_TYPE_COLON;
            Token->TextLength = Cursor - TokenStart;
            return TRUE;
        }

        "\n" {
            Token->Type = CHIP8_LEXER_TOKEN_TYPE_NEWLINE;
            Token->TextLength = Cursor - TokenStart;
            return TRUE;
        }

        "R"[0-9A-F] {
            Token->Type = CHIP8_LEXER_TOKEN_TYPE_REGISTER;
            Token->TextLength = Cursor - TokenStart;
            return TRUE;
        }

        [0-9A-F]+ {
            Token->Type = CHIP8_LEXER_TOKEN_TYPE_NUMBER;
            Token->TextLength = Cursor - TokenStart;
            return TRUE;
        }

        [A-Z_][A-Z0-9_]* {
            Token->Type = CHIP8_LEXER_TOKEN_TYPE_IDENTIFIER;
            Token->TextLength = Cursor - TokenStart;
            return TRUE;
        }

		[ \t\r]+ {
			goto next;
		}

        * {
            Token->Type = CHIP8_LEXER_TOKEN_TYPE_INVALID;
            Token->TextLength = Cursor - TokenStart;
            return FALSE;
        }
    */

    return FALSE;
}

VOID
Chip8LexerInitialize(
    _Inout_ CHIP8_LEXER* Lexer,
    _In_ LPCSTR Start,
    _In_ SIZE_T Length
)
{
	Lexer->Start = Start;
	Lexer->End = Start + Length;
	Lexer->Cursor = Start;
}

BOOL
Chip8LexerPeekToken(
    _In_ CONST CHIP8_LEXER CONST* Lexer,
    _Inout_ CHIP8_LEXER_TOKEN* Token
)
{
	return Chip8LexToken( Lexer->Cursor, Lexer->End - Lexer->Cursor, Token );
}

BOOL
Chip8LexerAdvanceToken(
    _Inout_ CHIP8_LEXER* Lexer,
    _Inout_opt_ CHIP8_LEXER_TOKEN* Token
)
{
	CHIP8_LEXER_TOKEN LexedToken = { };
	if ( Chip8LexToken( Lexer->Cursor, Lexer->End - Lexer->Cursor, &LexedToken ) == FALSE )
	{
		return FALSE;
	}

	Lexer->Cursor = LexedToken.TextStart + LexedToken.TextLength;

	if ( Token )
	{
		*Token = LexedToken;
	}

	return TRUE;
}