#include <interpreter/display.h>

BYTE
Chip8VirtualDisplayGetPixel(
    _In_ CONST CHIP8_VIRTUAL_DISPLAY CONST* Display,
    _In_ BYTE RowX,
    _In_ BYTE RowY
)
{
    CONST BYTE Index = RowX / 8;
    CONST BYTE Mask = 1 << ( 7 - ( RowX & 7 ) );

    return ( ( *Display )[ RowY ][ Index ] & Mask ) != 0;
}

VOID
Chip8VirtualDisplaySetPixel(
    _Inout_ CHIP8_VIRTUAL_DISPLAY* Display,
    _In_ BYTE RowX,
    _In_ BYTE RowY,
    _In_ BYTE Value
)
{
    CONST BYTE Index = RowX / 8;
    CONST BYTE Mask = 1 << ( 7 - ( RowX & 7 ) );

    if ( Value )
    {
        ( ( *Display )[ RowY ][ Index ] ) |= Mask;
    }
    else
    {
        ( ( *Display )[ RowY ][ Index ] ) &= ~Mask;
    }
}

VOID
Chip8VirtualDisplayClear(
    _Inout_ CHIP8_VIRTUAL_DISPLAY* Display
)
{
    memset( *Display, NULL, sizeof( *Display ) );
}