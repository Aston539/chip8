#include <assembler/assembler.h>
#include <encoder/encoder.h>
#include <disassembler/disassembler.h>

typedef struct _CHIP8_LEXER
{
    LPCSTR Source;
    SIZE_T Position;
    SIZE_T Length;

} CHIP8_LEXER, *PCHIP8_LEXER;

typedef struct _CHIP8_TOKEN
{
    LPCSTR Start;
    SIZE_T Length;

} CHIP8_TOKEN, *PCHIP8_TOKEN;

UINT16
Chip8AssembleStatement(
    _In_ LPCSTR Statement
)
{
    if ( stricmp( Statement, "CLS" ) == 0 )
    {
        return Chip8EncodeInstruction( CHIP8_OPCODE_KEY_SPECIAL_CLS, NULL, NULL, NULL, NULL );
    }
    else if ( stricmp( Statement, "RET" ) == 0 )
    {
        return Chip8EncodeInstruction( CHIP8_OPCODE_KEY_SPECIAL_RET, NULL, NULL, NULL, NULL );
    }
    else if ( stricmp( Statement, "JMP" ) == 0 )
    {

    }
    else if ( stricmp( Statement, "CALL" ) == 0 )
    {
        return Chip8EncodeInstruction( CHIP8_OPCODE_KEY_CALL, NULL, NULL, NULL, NULL );
    }
}