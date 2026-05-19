#ifndef _CHIP8_VIRTUAL_MACHINE_H_
#define _CHIP8_VIRTUAL_MACHINE_H_

#include "processor.h"
#include "display.h"
#include "keypad.h"

#define CHIP8_VIRTUAL_MACHINE_FONTSET_BASE 0x50

typedef struct _CHIP8_VIRTUAL_MACHINE
{
    BYTE Memory[ 4096 ];
    
    CHIP8_VIRTUAL_KEYPAD PreviousKeypad;
    CHIP8_VIRTUAL_KEYPAD Keypad;

    CHIP8_VIRTUAL_DISPLAY Display;
    CHIP8_VIRTUAL_PROCESSOR Processor;

} CHIP8_VIRTUAL_MACHINE, *PCHIP8_VIRTUAL_MACHINE;

VOID
Chip8VirtualMachineStartup(
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine
);

BOOL
Chip8VirtualMachineLoadProgram(
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine,
    _In_ CONST BYTE CONST* Program,
    _In_ SIZE_T ProgramSize
);

BOOL
Chip8VirtualMachineExecuteProgramCycle(
    _Inout_ CHIP8_VIRTUAL_MACHINE* Machine
);

#endif // _CHIP8_VIRTUAL_MACHINE_H_