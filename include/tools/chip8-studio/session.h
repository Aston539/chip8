#ifndef _CHIP8_STUDIO_SESSION_H_
#define _CHIP8_STUDIO_SESSION_H_

#include <defines.h>

#include <unordered_set>
#include <vector>

#include <QObject>
#include <QTimer>

#include <disassembler/disassembler.h>
#include <interpreter/machine.h>

class CHIP8_STUDIO_SESSION : public QObject
{
    Q_OBJECT

private:

    CHIP8_DISASSEMBLED_PROGRAM Disassembly;
    CHIP8_VIRTUAL_MACHINE VirtualMachine;
    QTimer ExecutionTimer;
    QTimer Timer60Hz;

    BOOL Running;

    std::unordered_set<UINT16> Breakpoints;

public:

    BOOL Setup( );

public:

    VOID Step( );
    VOID Run( );
    VOID Pause( );

public:

    CHIP8_VIRTUAL_MACHINE* GetMachine( );
    CHIP8_DISASSEMBLED_PROGRAM* GetDisassembly( );

public:

    VOID LoadProgram( CONST std::vector<BYTE>& Program );

public:

    BOOL HasBreakpoint( UINT16 Address );
    VOID ToggleBreakpoint( UINT16 Address );

signals:

    VOID Updated( );
    VOID Started( );
    VOID Paused( );
    VOID BreakpointHit( UINT16 Address );

private slots:

    VOID OnExecutionTick( );
    VOID OnTimerTick( );
};

#endif // _CHIP8_STUDIO_SESSION_H_