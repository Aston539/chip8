#include <iostream>
#include <fstream>
#include <vector>

#include <assembler/assembler.h>

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
        }
    }
}