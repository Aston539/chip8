#include <interpreter/machine.h>

#include <fstream>
#include <vector>

STATIC CHIP8_VIRTUAL_MACHINE Chip8VirtualMachine;
STATIC ULONG DisplayScale = 16;

LRESULT
CALLBACK
WindowProcedure(
    _In_ HWND Window,
    _In_ ULONG Message,
    _In_ WPARAM WParam,
    _In_ LPARAM LParam
)
{
    switch ( Message )
    {
        case WM_PAINT:
        {
            Chip8VirtualMachineExecuteProgramCycle( &Chip8VirtualMachine );

            PAINTSTRUCT Paint = { };
            HDC DeviceContext = BeginPaint( Window, &Paint );

            STATIC HBRUSH PrimaryColour = CreateSolidBrush( RGB( 255, 255, 255 ) );
            STATIC HBRUSH SecondaryColour = CreateSolidBrush( RGB( 0, 0, 0 ) );

            HGDIOBJ OriginalBrush = NULL;
            for ( BYTE Y = NULL; Y < 32; Y++ )
            {
                for ( BYTE X = NULL; X < 64; X++ )
                {
                    if ( Chip8VirtualDisplayGetPixel( &Chip8VirtualMachine.Display, X, Y ) )
                    {
                        OriginalBrush = SelectObject( DeviceContext, PrimaryColour );
                    }
                    else
                    {
                        OriginalBrush = SelectObject( DeviceContext, SecondaryColour );
                    }

                    Rectangle( DeviceContext, X * DisplayScale, Y * DisplayScale, ( X + 1 ) * DisplayScale, ( Y + 1 ) * DisplayScale );
                }
            }

            EndPaint( Window, &Paint );

            InvalidateRect( Window, NULL, TRUE );
            UpdateWindow( Window );

        } break;

        case WM_KEYDOWN:
        {
            switch ( WParam )
            {
                case '1': Chip8VirtualMachine.Keypad[ 0x1 ] = TRUE; break;
                case '2': Chip8VirtualMachine.Keypad[ 0x2 ] = TRUE; break;
                case '3': Chip8VirtualMachine.Keypad[ 0x3 ] = TRUE; break;
                case '4': Chip8VirtualMachine.Keypad[ 0xC ] = TRUE; break;

                case 'Q': Chip8VirtualMachine.Keypad[ 0x4 ] = TRUE; break;
                case 'W': Chip8VirtualMachine.Keypad[ 0x5 ] = TRUE; break;
                case 'E': Chip8VirtualMachine.Keypad[ 0x6 ] = TRUE; break;
                case 'R': Chip8VirtualMachine.Keypad[ 0xD ] = TRUE; break;

                case 'A': Chip8VirtualMachine.Keypad[ 0x7 ] = TRUE; break;
                case 'S': Chip8VirtualMachine.Keypad[ 0x8 ] = TRUE; break;
                case 'D': Chip8VirtualMachine.Keypad[ 0x9 ] = TRUE; break;
                case 'F': Chip8VirtualMachine.Keypad[ 0xE ] = TRUE; break;

                case 'Z': Chip8VirtualMachine.Keypad[ 0xA ] = TRUE; break;
                case 'X': Chip8VirtualMachine.Keypad[ 0x0 ] = TRUE; break;
                case 'C': Chip8VirtualMachine.Keypad[ 0xB ] = TRUE; break;
                case 'V': Chip8VirtualMachine.Keypad[ 0xF ] = TRUE; break;

                default: break;
            }
        
        } break;
        
        case WM_KEYUP:
        {
            switch ( WParam )
            {
                case '1': Chip8VirtualMachine.Keypad[ 0x1 ] = FALSE; break;
                case '2': Chip8VirtualMachine.Keypad[ 0x2 ] = FALSE; break;
                case '3': Chip8VirtualMachine.Keypad[ 0x3 ] = FALSE; break;
                case '4': Chip8VirtualMachine.Keypad[ 0xC ] = FALSE; break;

                case 'Q': Chip8VirtualMachine.Keypad[ 0x4 ] = FALSE; break;
                case 'W': Chip8VirtualMachine.Keypad[ 0x5 ] = FALSE; break;
                case 'E': Chip8VirtualMachine.Keypad[ 0x6 ] = FALSE; break;
                case 'R': Chip8VirtualMachine.Keypad[ 0xD ] = FALSE; break;

                case 'A': Chip8VirtualMachine.Keypad[ 0x7 ] = FALSE; break;
                case 'S': Chip8VirtualMachine.Keypad[ 0x8 ] = FALSE; break;
                case 'D': Chip8VirtualMachine.Keypad[ 0x9 ] = FALSE; break;
                case 'F': Chip8VirtualMachine.Keypad[ 0xE ] = FALSE; break;

                case 'Z': Chip8VirtualMachine.Keypad[ 0xA ] = FALSE; break;
                case 'X': Chip8VirtualMachine.Keypad[ 0x0 ] = FALSE; break;
                case 'C': Chip8VirtualMachine.Keypad[ 0xB ] = FALSE; break;
                case 'V': Chip8VirtualMachine.Keypad[ 0xF ] = FALSE; break;

                default: break;
            }
        
        } break;

        case WM_DESTROY:
        {
            PostQuitMessage( NULL );

            return NULL;

        } break;
    }

    return DefWindowProc( Window, Message, WParam, LParam );
}

BOOL
SetupVirtualMachine(
    VOID
)
{
    Chip8VirtualMachineStartup( &Chip8VirtualMachine );

    std::vector<BYTE> Program = { };
    std::ifstream File( "C:\\Users\\Aston\\Downloads\\Pong 2 (Pong hack) [David Winter, 1997].ch8", std::ios::in | std::ios::binary );
    if ( File.is_open( ) == FALSE )
    {
        return 1;
    }

    File.seekg( NULL, std::ios::end );
    std::streamsize FileSize = File.tellg( );
    File.seekg( NULL, std::ios::beg );

    Program.resize( FileSize );
    File.read( ( CHAR* )Program.data( ), FileSize );

    return Chip8VirtualMachineLoadProgram( &Chip8VirtualMachine, Program.data( ), Program.size( ) );
}

int main( )
{
    if ( SetupVirtualMachine( ) == FALSE )
    {
        return 1;
    }

    WNDCLASS WindowClass = { };
    WindowClass.lpfnWndProc = ( WNDPROC )WindowProcedure;
    WindowClass.lpszClassName = "INTC8";

    RegisterClass( &WindowClass );

    HWND Window = CreateWindowEx( NULL, 
                                  "INTC8", 
                                  "Chip8 Interpreter",
                                  WS_OVERLAPPEDWINDOW, 
                                  CW_USEDEFAULT, 
                                  CW_USEDEFAULT, 
                                  64 * DisplayScale, 
                                  32 * DisplayScale,
                                  NULL, 
                                  NULL, 
                                  NULL, 
                                  NULL );

    ShowWindow( Window, SW_SHOW );

    MSG WindowMessage = { };
    while ( GetMessage( &WindowMessage, NULL, NULL, NULL ) )
    {
        TranslateMessage( &WindowMessage );
        DispatchMessage( &WindowMessage );
    }
}