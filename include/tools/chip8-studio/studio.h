#ifndef _CHIP8_STUDIO_H_
#define _CHIP8_STUDIO_H_

#include <defines.h>

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTableView>
#include <QSplitter>

#include "session.h"
#include "displayview.h"
#include "disassemblymodel.h"
#include "memorymodel.h"
#include "callstackmodel.h"

class CHIP8_STUDIO : public QMainWindow
{
    Q_OBJECT

private:

    CHIP8_STUDIO_SESSION Session;

    CHIP8_STUDIO_DISPLAY_VIEW DisplayView;

    CHIP8_STUDIO_MEMORY_MODEL MemoryModel;
    CHIP8_STUDIO_DISASSEMBLY_MODEL DisassemblyModel;
    CHIP8_STUDIO_CALLSTACK_MODEL CallStackModel;

    QTableView MemoryView;
    QTableView DisassemblyView;
    QTableView CallStackView;

private:

    QAction* LoadProgramAction;
    QAction* RunAction;
    QAction* PauseAction;
    QAction* StepAction;

    QSplitter* MainSplitter;
    QSplitter* BottomSplitter;
    QSplitter* TopSplitter;

private:

    BOOL SetupWindow( );
    BOOL SetupActions( );
    BOOL SetupMenu( );
    BOOL SetupSession( );
    BOOL SetupViews( );
    BOOL SetupConnections( );

public:

    BOOL Setup( );

protected:

    VOID keyPressEvent( QKeyEvent* Event ) OVERRIDE;
    VOID keyReleaseEvent( QKeyEvent* Event ) OVERRIDE;

private:

    VOID LoadProgram( );

};

#endif // _CHIP8_STUDIO_H_