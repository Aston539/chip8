#include <tools/chip8-studio/studio.h>

#include <fstream>

#include <QFileDialog>
#include <QMenuBar>
#include <QHeaderView>
#include <QKeyEvent>

BOOL CHIP8_STUDIO::SetupWindow( )
{
    setWindowTitle( "CHIP-8 Studio" );
    resize( 800, 600 );

    return TRUE;
}

BOOL CHIP8_STUDIO::SetupActions( )
{
    LoadProgramAction = new QAction( tr( "&Load Program" ), this );
    LoadProgramAction->setShortcut( QKeySequence::Open );

    connect( LoadProgramAction, &QAction::triggered, this, &CHIP8_STUDIO::LoadProgram );

    RunAction = new QAction( tr( "&Run" ), this );
    PauseAction = new QAction( tr( "&Pause" ), this );
    StepAction = new QAction( tr( "&Step" ), this );

    connect( RunAction, &QAction::triggered, &Session, &CHIP8_STUDIO_SESSION::Run );
    connect( PauseAction, &QAction::triggered, &Session, &CHIP8_STUDIO_SESSION::Pause );
    connect( StepAction, &QAction::triggered, &Session, &CHIP8_STUDIO_SESSION::Step );

    return TRUE;
}

BOOL CHIP8_STUDIO::SetupMenu( )
{
    QMenu* FileMenu = menuBar( )->addMenu( "File" );
    FileMenu->addAction( LoadProgramAction );

    QMenu* DebugMenu = menuBar( )->addMenu( "Debug" );
    DebugMenu->addAction( RunAction );
    DebugMenu->addAction( PauseAction );
    DebugMenu->addAction( StepAction );

    return TRUE;
}

BOOL CHIP8_STUDIO::SetupSession( )
{
    return Session.Setup( );
}

BOOL CHIP8_STUDIO::SetupViews( )
{
    QFont PrimaryFont( "Consolas" );
    PrimaryFont.setStyleHint( QFont::Monospace );

    DisplayView.SetSession( &Session );

    MemoryModel.SetSession( &Session );
    MemoryView.setModel( &MemoryModel );
    MemoryView.setShowGrid( FALSE );
    MemoryView.verticalHeader( )->setVisible( FALSE );
    MemoryView.horizontalHeader( )->setVisible( FALSE );
    MemoryView.horizontalHeader( )->setDefaultSectionSize( 0 );
    MemoryView.setColumnWidth( 0, 80 );
    MemoryView.setFont( PrimaryFont );

    DisassemblyModel.SetSession( &Session );
    DisassemblyView.setModel( &DisassemblyModel );
    DisassemblyView.setShowGrid( FALSE );
    DisassemblyView.verticalHeader( )->setVisible( FALSE );
    DisassemblyView.horizontalHeader( )->setStretchLastSection( TRUE );
    DisassemblyView.setFont( PrimaryFont );
    DisassemblyView.setColumnWidth( 0, 80 );
    DisassemblyView.setColumnWidth( 1, 80 );
    DisassemblyView.setColumnWidth( 2, 120 );

    MemoryView.verticalHeader( )->hide( );
    DisassemblyView.verticalHeader( )->hide( );

    TopSplitter = new QSplitter( Qt::Horizontal );
    TopSplitter->addWidget( &DisassemblyView );
    TopSplitter->addWidget( &DisplayView );

    MainSplitter = new QSplitter( Qt::Vertical );
    MainSplitter->addWidget( TopSplitter );
    MainSplitter->addWidget( &MemoryView );

    setCentralWidget( MainSplitter );

    return TRUE;
}

BOOL CHIP8_STUDIO::SetupConnections( )
{
    connect( &Session,  &CHIP8_STUDIO_SESSION::Updated, &DisplayView, QOverload<>::of( &QWidget::update ) );
    connect( &Session, &CHIP8_STUDIO_SESSION::Updated, &MemoryModel, &CHIP8_STUDIO_MEMORY_MODEL::Refresh );
    connect( &Session, &CHIP8_STUDIO_SESSION::Updated, &DisassemblyModel, &CHIP8_STUDIO_DISASSEMBLY_MODEL::Refresh );

    return TRUE;
}

BOOL CHIP8_STUDIO::Setup( )
{
    if ( SetupWindow( ) == FALSE )
    {
        return FALSE;
    }

    if ( SetupActions( ) == FALSE )
    {
        return FALSE;
    }

    if ( SetupMenu( ) == FALSE )
    {
        return FALSE;
    }

    if ( SetupSession( ) == FALSE )
    {
        return FALSE;
    }

    if ( SetupViews( ) == FALSE )
    {
        return FALSE;
    }

    if ( SetupConnections( ) == FALSE )
    {
        return FALSE;
    }

    setFocusPolicy( Qt::StrongFocus );
    show( );

    return TRUE;
}

STATIC
LONG
MapQtKeyToChip8Key(
    _In_ LONG Key
)
{
    switch ( Key )
    {
        case Qt::Key_1: return 0x1;
        case Qt::Key_2: return 0x2;
        case Qt::Key_3: return 0x3;
        case Qt::Key_4: return 0xC;

        case Qt::Key_Q: return 0x4;
        case Qt::Key_W: return 0x5;
        case Qt::Key_E: return 0x6;
        case Qt::Key_R: return 0xD;

        case Qt::Key_A: return 0x7;
        case Qt::Key_S: return 0x8;
        case Qt::Key_D: return 0x9;
        case Qt::Key_F: return 0xE;

        case Qt::Key_Z: return 0xA;
        case Qt::Key_X: return 0x0;
        case Qt::Key_C: return 0xB;
        case Qt::Key_V: return 0xF;
    }

    return -1;
}

VOID CHIP8_STUDIO::keyPressEvent( QKeyEvent* Event )
{
    LONG Key = MapQtKeyToChip8Key( Event->key( ) );
    if ( Key >= 0 )
    {
        Session.GetMachine( )->Keypad[ Key ] = TRUE;
    }

    QMainWindow::keyPressEvent( Event );
}

VOID CHIP8_STUDIO::keyReleaseEvent( QKeyEvent* Event )
{
    LONG Key = MapQtKeyToChip8Key( Event->key( ) );
    if ( Key >= 0 )
    {
        Session.GetMachine( )->Keypad[ Key ] = FALSE;
    }

    QMainWindow::keyPressEvent( Event );
}

VOID CHIP8_STUDIO::LoadProgram( )
{
    QString ProgramFilePath = QFileDialog::getOpenFileName( this, "Load CHIP-8 Program", "", "CHIP-8 Programs (*.ch8);;All Files (*)" );
    if ( ProgramFilePath.isEmpty( ) )
    {
        return;
    }

    std::vector<BYTE> Program = { };
    std::ifstream File( ProgramFilePath.toStdString( ), std::ios::in | std::ios::binary );
    if ( File.is_open( ) == FALSE )
    {
        return;
    }

    File.seekg( NULL, std::ios::end );
    std::streamsize FileSize = File.tellg( );
    File.seekg( NULL, std::ios::beg );

    Program.resize( FileSize );
    File.read( ( CHAR* )Program.data( ), FileSize );

    Session.LoadProgram( Program );
    DisassemblyView.update( );
    DisassemblyModel.Rebuild( );
}