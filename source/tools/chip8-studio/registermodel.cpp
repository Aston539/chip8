#include <tools/chip8-studio/registermodel.h>

INT CHIP8_STUDIO_REGISTER_MODEL::rowCount( CONST QModelIndex& Parent ) CONST
{
    //
    // registers + memory index + delay timer + sound timer + program counter
    //
    return 16 + 1 + 1 + 1 + 1;
}

INT CHIP8_STUDIO_REGISTER_MODEL::columnCount( CONST QModelIndex& Parent ) CONST
{
    return 2;
}

QVariant CHIP8_STUDIO_REGISTER_MODEL::data( CONST QModelIndex& Index, INT Role ) CONST
{
    if ( Role != Qt::DisplayRole )
    {
        return QVariant( );
    }

    switch ( Index.column( ) )
    {
        case 0:
        {
            if ( Index.row( ) < 16 )
            {
                return Index.row( );
            }

            switch ( Index.row( ) - 16 )
            {
                case 0: return "MEMORY INDEX";
                case 1: return "DELAY TIMER";
                case 2: return "SOUND TIMER";
                case 3: return "PROGRAM COUNTER";
                default: break;
            }

        } break;


        case 1:
        {
            UINT16 Value = NULL;
            if ( Index.row( ) < 16 )
            {
                Value = Session->GetMachine( )->Processor.Registers[ Index.row( ) ];
            }
            else
            {
                switch ( Index.row( ) - 16 )
                {
                    case 0: Value = Session->GetMachine( )->Processor.MemoryIndex; break;
                    case 1: Value = Session->GetMachine( )->Processor.DelayTimer; break;
                    case 2: Value = Session->GetMachine( )->Processor.SoundTimer; break;
                    case 3: Value = Session->GetMachine( )->Processor.ProgramCounter; break;
                    default: return "";
                }
            }

            return QString( "%1" ).arg( Value, 4, 16, QChar( '0' ) ).toUpper( );

        } break;

        default: break;
    }

    return QVariant( );
}

QVariant CHIP8_STUDIO_REGISTER_MODEL::headerData( INT Section, Qt::Orientation Orientation, INT Role ) CONST
{
    if ( Role != Qt::DisplayRole || Orientation != Qt::Horizontal )
    {
        return QVariant( );
    }

    switch ( Section )
    {
        case 0: return "Register";
        case 1: return "Value";
        default: break;
    }

    return QVariant( );
}

VOID CHIP8_STUDIO_REGISTER_MODEL::Refresh( )
{
    emit dataChanged( createIndex( 0, 0 ), createIndex( rowCount( ) - 1, columnCount( ) - 1 ) );
}