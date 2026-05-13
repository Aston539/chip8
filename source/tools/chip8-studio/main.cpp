#include <cstdlib>

#include <fstream>
#include <vector>
#include <array>

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QMenuBar>
#include <QFileDialog>

#include <interpreter/machine.h>
#include <disassembler/disassembler.h>

#include <tools/chip8-studio/studio.h>

int main( int argc, char* argv[ ]  )
{
    QApplication Application( argc, argv );

    CHIP8_STUDIO Studio = { };
    Studio.Setup( );

    return Application.exec( );
}