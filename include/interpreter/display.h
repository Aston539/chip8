#ifndef _CHIP8_VIRTUAL_DISPLAY_H_
#define _CHIP8_VIRTUAL_DISPLAY_H_

#include <defines.h>

//
// 64x32 bits
//
typedef BYTE CHIP8_VIRTUAL_DISPLAY[ 32 ][ 8 ];

BYTE
Chip8VirtualDisplayGetPixel(
    _In_ CONST CHIP8_VIRTUAL_DISPLAY CONST* Display,
    _In_ BYTE RowX,
    _In_ BYTE RowY
);

VOID
Chip8VirtualDisplaySetPixel(
    _Inout_ CHIP8_VIRTUAL_DISPLAY* Display,
    _In_ BYTE RowX,
    _In_ BYTE RowY,
    _In_ BYTE Value
);

VOID
Chip8VirtualDisplayClear(
    _Inout_ CHIP8_VIRTUAL_DISPLAY* Display
);

#endif // _CHIP8_VIRTUAL_DISPLAY_H_