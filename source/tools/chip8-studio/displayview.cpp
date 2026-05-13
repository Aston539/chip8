#include <tools/chip8-studio/displayview.h>

#include <QPainter>

VOID CHIP8_STUDIO_DISPLAY_VIEW::paintEvent( QPaintEvent* PaintEvent )
{
    QPainter Painter( this );
    Painter.fillRect( rect( ), Qt::black );

    Painter.setBrush( Qt::white );
    Painter.setPen( Qt::NoPen );

    LONG PixelScaleX = width( ) / 64;
    LONG PixelScaleY = height( ) / 32;

    for ( ULONG Y = NULL; Y < 32; Y++ )
    {
        for ( ULONG X = NULL; X < 64; X++ )
        {
            if ( Chip8VirtualDisplayGetPixel( &Session->GetMachine( )->Display, X, Y ) )
            {
                Painter.drawRect( X * PixelScaleX, Y * PixelScaleY, PixelScaleX, PixelScaleY );
            }
        }
    }
}