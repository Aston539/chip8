#include <tools/chip8-studio/callstackmodel.h>

INT CHIP8_STUDIO_CALLSTACK_MODEL::rowCount( CONST QModelIndex& Parent ) CONST
{
    return 16;
}

INT CHIP8_STUDIO_CALLSTACK_MODEL::columnCount( CONST QModelIndex& Parent ) CONST
{
    return 2;
}

QVariant CHIP8_STUDIO_CALLSTACK_MODEL::data( CONST QModelIndex& Index, INT Role ) CONST
{
    if ( Role != Qt::DisplayRole )
    {
        return QVariant( );
    }

    if ( Index.row( ) >= Session->GetMachine( )->Processor.CallStackSize )
    {
        return "";
    }

    switch ( Index.column( ) )
    {
        case 0: return Index.row( );
        case 1: return QString( "%1" ).arg( Session->GetMachine( )->Processor.CallStack[ Index.row( ) ], 4, 16, QChar( '0' ) ).toUpper( );
        default: break;
    }

    return QVariant( );
}

QVariant CHIP8_STUDIO_CALLSTACK_MODEL::headerData( INT Section, Qt::Orientation Orientation, INT Role ) CONST
{
    if ( Role != Qt::DisplayRole || Orientation != Qt::Horizontal )
    {
        return QVariant( );
    }

    switch ( Section )
    {
        case 0: return "Index";
        case 1: return "Address";
        default: break;
    }

    return QVariant( );
}

VOID CHIP8_STUDIO_CALLSTACK_MODEL::Refresh( )
{
    emit dataChanged( createIndex( 0, 0 ), createIndex( rowCount( ) - 1, columnCount( ) - 1 ) );
}