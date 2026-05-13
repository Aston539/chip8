#ifndef _CHIP8_STUDIO_MEMORY_MODEL_H_
#define _CHIP8_STUDIO_MEMORY_MODEL_H_

#include <defines.h>

#include <QAbstractTableModel>
#include <QVector>
#include <QString>
#include <QVariant>

#include "sessionview.h"

class CHIP8_STUDIO_MEMORY_MODEL : public QAbstractTableModel, 
                                  public CHIP8_STUDIO_SESSION_VIEW
{
    Q_OBJECT

public:

    INT rowCount( CONST QModelIndex& Parent = QModelIndex( ) ) CONST OVERRIDE;
    INT columnCount( CONST QModelIndex& Parent = QModelIndex( ) ) CONST OVERRIDE;
    
    QVariant data( CONST QModelIndex& Index, INT Role ) CONST OVERRIDE;

public slots:

    VOID Refresh( );
};

#endif // _CHIP8_STUDIO_MEMORY_MODEL_H_