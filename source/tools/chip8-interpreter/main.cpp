#include <interpreter/machine.h>
#include <disassembler/disassembler.h>

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
            PAINTSTRUCT Paint = { };
            HDC DeviceContext = BeginPaint( Window, &Paint );

            STATIC HBRUSH PrimaryColour = CreateSolidBrush( RGB( 0, 160, 255 ) );
            STATIC HBRUSH SecondaryColour = CreateSolidBrush( RGB( 255, 85, 0 ) );

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

                    Rectangle( DeviceContext, 
                               ( X * DisplayScale ),
                               ( Y * DisplayScale ), 
                               ( ( X + 1 ) * DisplayScale ), 
                               ( ( Y + 1 ) * DisplayScale ) );
                }
            }

            EndPaint( Window, &Paint );

        } break;

        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            BOOL State = ( Message == WM_KEYDOWN );

            switch ( WParam )
            {
                case '1': Chip8VirtualMachine.Keypad[ 0x1 ] = State; break;
                case '2': Chip8VirtualMachine.Keypad[ 0x2 ] = State; break;
                case '3': Chip8VirtualMachine.Keypad[ 0x3 ] = State; break;
                case '4': Chip8VirtualMachine.Keypad[ 0xC ] = State; break;

                case 'Q': case 'q': Chip8VirtualMachine.Keypad[ 0x4 ] = State; break;
                case 'W': case 'w': Chip8VirtualMachine.Keypad[ 0x5 ] = State; break;
                case 'E': case 'e': Chip8VirtualMachine.Keypad[ 0x6 ] = State; break;
                case 'R': case 'r': Chip8VirtualMachine.Keypad[ 0xD ] = State; break;

                case 'A': case 'a': Chip8VirtualMachine.Keypad[ 0x7 ] = State; break;
                case 'S': case 's': Chip8VirtualMachine.Keypad[ 0x8 ] = State; break;
                case 'D': case 'd': Chip8VirtualMachine.Keypad[ 0x9 ] = State; break;
                case 'F': case 'f': Chip8VirtualMachine.Keypad[ 0xE ] = State; break;

                case 'Z': case 'z': Chip8VirtualMachine.Keypad[ 0xA ] = State; break;
                case 'X': case 'x': Chip8VirtualMachine.Keypad[ 0x0 ] = State; break;
                case 'C': case 'c': Chip8VirtualMachine.Keypad[ 0xB ] = State; break;
                case 'V': case 'v': Chip8VirtualMachine.Keypad[ 0xF ] = State; break;

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
    //std::ifstream File( "C:\\Users\\Aston\\Downloads\\Pong 2 (Pong hack) [David Winter, 1997].ch8", std::ios::in | std::ios::binary );
    //std::ifstream File( "C:\\Users\\Aston\\Downloads\\Hi - Lo[ Jef Winsor, 1978 ].ch8", std::ios::in | std::ios::binary );
    //std::ifstream File( "C:\\Users\\Aston\\Downloads\\Space Invaders [David Winter].ch8", std::ios::in | std::ios::binary );
    //std::ifstream File( "C:\\Users\\Aston\\Downloads\\Connect 4 [David Winter].ch8", std::ios::in | std::ios::binary );
    //std::ifstream File( "C:\\Users\\Aston\\Documents\\Projects\\aston-work\\chip8\\roms\\1-chip8-logo.ch8", std::ios::in | std::ios::binary );
    //std::ifstream File( "C:\\Users\\Aston\\Documents\\Projects\\aston-work\\chip8\\roms\\2-ibm-logo.ch8", std::ios::in | std::ios::binary );
    //std::ifstream File( "C:\\Users\\Aston\\Documents\\Projects\\aston-work\\chip8\\roms\\3-corax+.ch8", std::ios::in | std::ios::binary );
    std::ifstream File( "C:\\Users\\Aston\\Documents\\Projects\\aston-work\\chip8\\roms\\4-flags.ch8", std::ios::in | std::ios::binary );
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

#include <thread>

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
                                  ( 64 * DisplayScale ) + ( DisplayScale * 2 ), 
                                  ( 32 * DisplayScale ) + ( DisplayScale * 2 ),
                                  NULL, 
                                  NULL, 
                                  NULL, 
                                  NULL );

    ShowWindow( Window, SW_SHOW );

    std::thread( [& ]( ) 
    {
        while ( TRUE )
        {
            InvalidateRect( Window, NULL, TRUE );
            UpdateWindow( Window );

            for ( ULONG I = NULL; I < 16; I ++ ) Chip8VirtualMachineExecuteProgramCycle( &Chip8VirtualMachine );

            Sleep( 1 );
        }

        //UINT16 Address = Chip8VirtualMachine.Processor.ProgramCounter;
        //
        //CHIP8_INSTRUCTION CurrentInstruction = { };
        //Chip8DisassembleInstruction( ( CONST UINT16 CONST* ) & Chip8VirtualMachine.Memory[ Address ], &CurrentInstruction );
        //
        //BYTE InstructionLow = Chip8VirtualMachine.Memory[ Address ];
        //BYTE InstructionHigh = Chip8VirtualMachine.Memory[ Address + 1 ];
        //
        //CHAR InstructionFormat[ 64 + 1 ] = { };
        //Chip8FormatInstruction( &CurrentInstruction, InstructionFormat, 64 );
        //
        //printf( "%04X %02X %02X %s\n", Address, InstructionLow, InstructionHigh, InstructionFormat );
    } ).detach( );

    while ( TRUE )
    {
        MSG WindowMessage = { };
        if ( PeekMessage( &WindowMessage, NULL, NULL, NULL, PM_REMOVE ) )
        {
            if ( WindowMessage.message == WM_QUIT )
            {
                break;
            }

            TranslateMessage( &WindowMessage );
            DispatchMessage( &WindowMessage );
        }
    }
}