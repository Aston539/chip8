#include <assembler/assembler.h>
#include <encoder/encoder.h>
#include <disassembler/disassembler.h>

#define CHIP8_ASSEMBLER_TOKEN_TYPE_UNKNOWN  0
#define CHIP8_ASSEMBLER_TOKEN_TYPE_MNEMONIC 1
#define CHIP8_ASSEMBLER_TOKEN_TYPE_REGSTER  2
#define CHIP8_ASSEMBLER_TOKEN_TYPE_INTEGRAL 3

typedef struct _CHIP8_TOKEN
{
    BYTE Type;

    LPCSTR Lexeme;

    ULONG Line;
    ULONG Column;

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