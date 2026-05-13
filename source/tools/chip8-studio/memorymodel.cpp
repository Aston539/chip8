#include <tools/chip8-studio/memorymodel.h>

INT CHIP8_STUDIO_MEMORY_MODEL::rowCount( CONST QModelIndex& Parent ) CONST
{
    return 256;
}

INT CHIP8_STUDIO_MEMORY_MODEL::columnCount( CONST QModelIndex& Parent ) CONST
{
    return 17;
}

QVariant CHIP8_STUDIO_MEMORY_MODEL::data( CONST QModelIndex& Index, INT Role ) CONST
{
    if( Role != Qt::DisplayRole )
    {
        return QVariant( );
    }

    if ( Index.column( ) == 0 )
    {
        UINT16 Address = Index.row( ) * 16;
        return QString( "%1" ).arg( Address, 4, 16, QChar( '0' ) ).toUpper( );
    }

    UINT16 Address = ( Index.row( ) * 16 ) + ( Index.column( ) - 1 );
    BYTE Value = Session->GetMachine( )->Memory[ Address ];

    return QString( "%1" ).arg( Value, 2, 16, QChar( '0' ) ).toUpper( );
}

VOID CHIP8_STUDIO_MEMORY_MODEL::Refresh( )
{
    emit dataChanged( createIndex( 0, 0 ), createIndex( rowCount( ) - 1, columnCount( ) - 1 ) );
}