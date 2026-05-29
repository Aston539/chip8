#ifndef _CHIP8_VIRTUAL_PROCESSOR_H_
#define _CHIP8_VIRTUAL_PROCESSOR_H_

#include <isa/isa.h>

typedef struct _CHIP8_VIRTUAL_PROCESSOR
{
    CHIP8_ADDRESS ProgramCounter;
    UINT16 CycleCount;

    BYTE DelayTimer;
    BYTE SoundTimer;

    CHIP8_ADDRESS MemoryIndex;

    CHIP8_REGISTER Registers[ 16 ];

    CHIP8_ADDRESS CallStack[ 16 ];
    BYTE CallStackSize;

} CHIP8_VIRTUAL_PROCESSOR, *PCHIP8_VIRTUAL_PROCESSOR;

VOID
Chip8VirtualProcessorInitialise(
    _Inout_ CHIP8_VIRTUAL_PROCESSOR* Processor
);

BOOL
Chip8VirtualProcessorExecuteCycle(
    _Inout_ CHIP8_VIRTUAL_PROCESSOR* Processor,
    _Inout_ struct _CHIP8_VIRTUAL_MACHINE* Machine
);

#endif // _CHIP8_VIRTUAL_PROCESSOR_H_