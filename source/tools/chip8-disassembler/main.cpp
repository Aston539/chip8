#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>

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
    CHIP8_DISASSEMBLED_PROGRAM ProgramDisassembly = { };
    if ( Chip8DisassembleProgram( Code, CodeSize, &ProgramDisassembly ) == FALSE )
    {
        __debugbreak( );
    }

    for ( CONST CHIP8_MACHINE_FUNCTION& Function : ProgramDisassembly.Functions )
    {
        printf( "CODE:%04X", Function.Address );
        printf( "  " );
        printf( "FUNCTION_%04X START", Function.Address );
        printf( "\n" );

        BYTE BasicBlockIndex = NULL;
        for ( CONST CHIP8_MACHINE_BASIC_BLOCK& BasicBlock : Function.BasicBlocks | std::views::values )
        {
            if ( BasicBlock.Address != Function.Address )
            {
                printf( "CODE:%04X", BasicBlock.Address );
                printf( "  " );
                printf( "LABEL_%04X:", BasicBlock.Address );

                CHIP8_CONTROL_FLOW_NODE* ReferencingNode = NULL;
                if ( ( ( CHIP8_MACHINE_FUNCTION& )Function ).ControlFlowGraph.LookupNodeByAddress( BasicBlock.Address, &ReferencingNode ) )
                {
                    if ( ReferencingNode->Predecessors.empty( ) == FALSE )
                    {
                        printf( "\t" );
                        printf( "\t" );
                        printf( "\t" );
                        printf( "\t" );
                        printf( " ; -> " );

                        BYTE PredecessorIndex = NULL;
                        for ( CONST CHIP8_ADDRESS& Predecessor : ReferencingNode->Predecessors )
                        {
                            printf( "%04X", Predecessor );

                            if ( ( PredecessorIndex + 1 ) < ReferencingNode->Predecessors.size( ) )
                            {
                                printf( ", " );
                            }

                            PredecessorIndex += 1;
                        }
                    }
                }

                printf( "\n" );
            }

            for ( BYTE InstructionIndex = NULL;
                       InstructionIndex < BasicBlock.Instructions.size( );
                       InstructionIndex++ )
            {
                UINT16 InstructionAddress = BasicBlock.Address + ( InstructionIndex * sizeof( CHIP8_ENCODED_INSTRUCTION ) );
                CHAR InstructionText[ 64 + 1 ] = { };
                Chip8FormatInstruction( &BasicBlock.Instructions[ InstructionIndex ], InstructionText, 64 );

                printf( "CODE:%04X", InstructionAddress );
                printf( "    " );
                printf( "%02X %02X", Code[ InstructionAddress - 512 - 1 ], Code[ InstructionAddress - 512 ] );
                printf( "    " );
                printf( InstructionText );
                printf( "\n" );
            }

            if ( BasicBlockIndex != ( Function.BasicBlocks.size( ) - 1 ) )
            {
                printf( "\n" );
            }

            BasicBlockIndex += 1;
        }

        printf( "CODE:%04X", Function.Address );
        printf( "  " );
        printf( "FUNCTION_%04X END", Function.Address );
        printf( "\n" );

        printf( "\n" );
    }
}

int main( int ArgumentCount, char** Arguments )
{
    std::vector<BYTE> Code = { };

    if ( ArgumentCount < 2 )
    {
        //DisplayUsage( );
        
        //return 1;


        //std::ifstream File( "C:\\Users\\Aston\\Documents\\Projects\\aston-work\\chip8\\roms\\IBM Logo.ch8", std::ios::in | std::ios::binary );
        //std::ifstream File( "C:\\Users\\Aston\\Documents\\Projects\\aston-work\\chip8\\roms\\3-corax+.ch8", std::ios::in | std::ios::binary );
        std::ifstream File( "C:\\Users\\Aston\\Downloads\\Connect 4 [David Winter].ch8", std::ios::in | std::ios::binary );
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

    //std::vector<BYTE> Code = { };
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

    while ( getchar( ) == 0 );

    return 0;
}