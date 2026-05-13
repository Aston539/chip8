#ifndef _CHIP8_STUDIO_DISASSEMBLY_MODEL_H_
#define _CHIP8_STUDIO_DISASSEMBLY_MODEL_H_

#include <defines.h>

#include <QAbstractTableModel>

#include "sessionview.h"

#define CHIP8_STUDIO_DISASSEMBLY_ROW_TYPE_EMPTY          0
#define CHIP8_STUDIO_DISASSEMBLY_ROW_TYPE_FUNCTION_START 1
#define CHIP8_STUDIO_DISASSEMBLY_ROW_TYPE_FUNCTION_END   2
#define CHIP8_STUDIO_DISASSEMBLY_ROW_TYPE_LABEL          3
#define CHIP8_STUDIO_DISASSEMBLY_ROW_TYPE_INSTRUCTION    4

typedef struct _CHIP8_STUDIO_DISASSEMBLY_ROW
{
    BYTE Type;

    UINT16 Address;

    QString AddressText;
    QString BytesText;
    QString InstructionText;
    
} CHIP8_STUDIO_DISASSEMBLY_ROW, *PCHIP8_STUDIO_DISASSEMBLY_ROW;

class CHIP8_STUDIO_DISASSEMBLY_MODEL : public QAbstractTableModel, 
                                       public CHIP8_STUDIO_SESSION_VIEW
{
    Q_OBJECT

private:

    std::vector<CHIP8_STUDIO_DISASSEMBLY_ROW> Rows;

public:

    VOID Rebuild( );

public:

    INT rowCount( CONST QModelIndex& Parent = QModelIndex( ) ) CONST OVERRIDE;
    INT columnCount( CONST QModelIndex& Parent = QModelIndex( ) ) CONST OVERRIDE;

    QVariant data( CONST QModelIndex& Index, INT Role ) CONST OVERRIDE;
    QVariant headerData( INT Section, Qt::Orientation Orientation, INT Role ) CONST OVERRIDE;

public slots:

    VOID Refresh( );
};

#endif // _CHIP8_STUDIO_DISASSEMBLY_MODEL_H_