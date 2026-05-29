#include <tools/chip8-studio/session.h>

BOOL CHIP8_STUDIO_SESSION::Setup( )
{
    ExecutionTimer.setInterval( 1000 / 60 );
    connect( &ExecutionTimer, &QTimer::timeout, this, &CHIP8_STUDIO_SESSION::OnExecutionTick );

    Timer60Hz.setInterval( 1000 / 60 );
    connect( &Timer60Hz, &QTimer::timeout, this, &CHIP8_STUDIO_SESSION::OnTimerTick );

    Running = FALSE;

    return TRUE;
}


VOID CHIP8_STUDIO_SESSION::Step( )
{
    //
    // we want to ignore breakpoints when stepping as its already fine grained execution control
    // 
    //if ( Breakpoints.contains( VirtualMachine.Processor.ProgramCounter ) )
    //{
    //    emit BreakpointHit( VirtualMachine.Processor.ProgramCounter );
    //
    //    Pause( );
    //
    //    return;
    //}

    Chip8VirtualMachineExecuteProgramCycle( &VirtualMachine );

    emit Updated( );
}

VOID CHIP8_STUDIO_SESSION::Run( )
{
    if ( Running )
    {
        return;
    }

    //
    // if program execution has been resumed on a breakpoint, step over the breakpoint
    //
    if ( Breakpoints.contains( VirtualMachine.Processor.ProgramCounter ) )
    {
        Step( );
    }

    Running = TRUE;

    ExecutionTimer.start( );
    Timer60Hz.start( );

    emit Started( );
}

VOID CHIP8_STUDIO_SESSION::Pause( )
{
    Running = FALSE;

    ExecutionTimer.stop( );
    Timer60Hz.stop( );

    emit Paused( );
}


CHIP8_VIRTUAL_MACHINE* CHIP8_STUDIO_SESSION::GetMachine( )
{
    return &VirtualMachine;
}

CHIP8_DISASSEMBLED_PROGRAM* CHIP8_STUDIO_SESSION::GetDisassembly( )
{
    return &Disassembly;
}

VOID CHIP8_STUDIO_SESSION::LoadProgram( CONST std::vector<BYTE>& Program )
{
    Pause( );

    Disassembly.Functions.clear( );

    Chip8DisassembleProgram( Program.data( ), Program.size( ), &Disassembly );

    Chip8VirtualMachineStartup( &VirtualMachine );
    Chip8VirtualMachineLoadProgram( &VirtualMachine, Program.data( ), Program.size( ) );
}

BOOL CHIP8_STUDIO_SESSION::HasBreakpoint( UINT16 Address )
{
    return Breakpoints.contains( Address );
}

VOID CHIP8_STUDIO_SESSION::ToggleBreakpoint( UINT16 Address )
{
    if ( Breakpoints.contains( Address ) )
    {
        Breakpoints.erase( Address );
    }
    else
    {
        Breakpoints.insert( Address );
    }

    emit Updated( );
}

VOID CHIP8_STUDIO_SESSION::OnExecutionTick( )
{
    for ( ULONG Cycle = NULL; Cycle < 10; Cycle++ )
    {
        if ( Breakpoints.contains( VirtualMachine.Processor.ProgramCounter ) )
        {
            emit BreakpointHit( VirtualMachine.Processor.ProgramCounter );

            Pause( );

            break;
        }

        Chip8VirtualMachineExecuteProgramCycle( &VirtualMachine );
    }

    emit Updated( );
}

VOID CHIP8_STUDIO_SESSION::OnTimerTick( )
{
    if ( VirtualMachine.Processor.DelayTimer > 0 )
    {
        VirtualMachine.Processor.DelayTimer -= 1;
    }

    if ( VirtualMachine.Processor.SoundTimer > 0 )
    {
        VirtualMachine.Processor.SoundTimer -= 1;
    }
}