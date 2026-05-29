#include <tools/chip8-studio/disassemblymodel.h>

#include <QColor>

#include <ranges>

VOID CHIP8_STUDIO_DISASSEMBLY_MODEL::Rebuild( )
{
    beginResetModel( );

    CONST CHIP8_DISASSEMBLED_PROGRAM* Program = Session->GetDisassembly( );

    Rows.clear( );
    for ( SIZE_T FunctionIndex = NULL;
                 FunctionIndex < Program->Functions.size( );
                 FunctionIndex++ )
    {
        CONST CHIP8_MACHINE_FUNCTION& Function = Program->Functions[ FunctionIndex ];

        CHIP8_STUDIO_DISASSEMBLY_ROW FunctionStartRow = { };
        FunctionStartRow.Type = CHIP8_STUDIO_DISASSEMBLY_ROW_TYPE_FUNCTION_START;
        FunctionStartRow.AddressText = QString( "FUNCTION %1" ).arg( Function.Address, 4, 16, QChar( '0' ) ).toUpper( );
        FunctionStartRow.InstructionText = "FUNCTION START";
        Rows.push_back( FunctionStartRow );

        for ( CONST CHIP8_MACHINE_BASIC_BLOCK& BasicBlock : Function.BasicBlocks | std::views::values )
        {
            CHIP8_STUDIO_DISASSEMBLY_ROW LabelRow = { };
            LabelRow.Type = CHIP8_STUDIO_DISASSEMBLY_ROW_TYPE_LABEL;
            LabelRow.Address = BasicBlock.Address;
            LabelRow.AddressText = QString( "%1" ).arg( BasicBlock.Address, 4, 16, QChar( '0' ) ).toUpper( );
            LabelRow.InstructionText = "LABEL";
            Rows.push_back( LabelRow );

            for ( SIZE_T InstructionIndex = NULL;
                         InstructionIndex < BasicBlock.Instructions.size( );
                         InstructionIndex++ )
            {
                //
                // hacky calculation of instruction address but should hold up for the use cases of a basic block
                // and machine instructions
                //
                CHIP8_ADDRESS InstructionAddress = BasicBlock.Address + ( InstructionIndex * sizeof( CHIP8_ENCODED_INSTRUCTION ) );
                UINT16 InstructionOpcode = *( UINT16* )&Session->GetMachine( )->Memory[ InstructionAddress ];

                CHIP8_STUDIO_DISASSEMBLY_ROW InstructionRow = { };
                InstructionRow.Type = CHIP8_STUDIO_DISASSEMBLY_ROW_TYPE_INSTRUCTION;
                InstructionRow.Address = InstructionAddress;
                InstructionRow.AddressText = QString( "%1" ).arg( InstructionAddress, 4, 16, QChar( '0' ) ).toUpper( );
                InstructionRow.BytesText = QString( "%1" ).arg( InstructionOpcode, 4, 16, QChar( '0' ) ).toUpper( );

                CHAR InstructionFormat[ 64 + 1 ] = { };
                if ( Chip8FormatInstruction( &BasicBlock.Instructions[ InstructionIndex ], InstructionFormat, 64 ) == FALSE )
                {
                    InstructionRow.InstructionText = QString( InstructionFormat );
                }
                else
                {
                    InstructionRow.InstructionText = QString( InstructionFormat );
                }

                Rows.push_back( InstructionRow );
            }
        }

        CHIP8_STUDIO_DISASSEMBLY_ROW FunctionEndRow = { };
        FunctionEndRow.Type = CHIP8_STUDIO_DISASSEMBLY_ROW_TYPE_FUNCTION_START;
        FunctionEndRow.InstructionText = "FUNCTION END";
        Rows.push_back( FunctionEndRow );
    }

    endResetModel( );
}

INT CHIP8_STUDIO_DISASSEMBLY_MODEL::rowCount( CONST QModelIndex& Parent ) CONST
{
    return Rows.size( );
}

INT CHIP8_STUDIO_DISASSEMBLY_MODEL::columnCount( CONST QModelIndex& Parent ) CONST
{
    return 4;
}

QVariant CHIP8_STUDIO_DISASSEMBLY_MODEL::data( CONST QModelIndex& Index, INT Role ) CONST
{
    if ( Index.isValid( ) == FALSE )
    {
        return QVariant( );
    }

    CONST CHIP8_STUDIO_DISASSEMBLY_ROW& Row = Rows[ Index.row( ) ];

    if ( Role == Qt::DisplayRole )
    {
        switch ( Index.column( ) )
        {
            case 0:
            {
                if ( Session->HasBreakpoint( Row.Address ) )
                {
                    return "●";
                }

                return "";
            }


            case 1: return Row.AddressText;
            case 2: return Row.BytesText;
            case 3: return Row.InstructionText;
        }
    }

    if ( Role == Qt::BackgroundRole )
    {
        if ( Row.Address == Session->GetMachine( )->Processor.ProgramCounter )
        {
            return QColor( 255, 255, 255, 75 );
        }
        else if ( Session->HasBreakpoint( Row.Address ) )
        {
            return QColor( 255, 50, 50, 75 );
        }
    }

    return QVariant( );
}

QVariant CHIP8_STUDIO_DISASSEMBLY_MODEL::headerData( INT Section, Qt::Orientation Orientation, INT Role ) CONST
{
    if ( Role != Qt::DisplayRole )
    {
        return QVariant( );
    }

    if ( Orientation != Qt::Horizontal )
    {
        return QVariant( );
    }

    switch ( Section )
    {
        case 0: return "";
        case 1: return "Address";
        case 2: return "Opcode";
        case 3: return "Instruction";
    }

    return QVariant( );
}

VOID CHIP8_STUDIO_DISASSEMBLY_MODEL::OnCellClicked( CONST QModelIndex& Index )
{
    if ( Index.column( ) != NULL )
    {
        return;
    }

    CHIP8_STUDIO_DISASSEMBLY_MODEL* Model = ( CHIP8_STUDIO_DISASSEMBLY_MODEL* )Index.model( );
    CONST CHIP8_STUDIO_DISASSEMBLY_ROW& Row = Model->Rows.at( Index.row( ) );

    Session->ToggleBreakpoint( Row.Address );

    emit Model->dataChanged( Index, Index );
}

VOID CHIP8_STUDIO_DISASSEMBLY_MODEL::Refresh( )
{
    Rebuild( );
}